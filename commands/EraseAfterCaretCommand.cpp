#include "EraseAfterCaretCommand.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../glyphs/NoteWidthCache.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../CaretNavigator.h"
#include "../NoteWrapper.h"
#include "../PageManager.h"
#include "../ByteChecker.h"

#pragma warning(disable:4996)

EraseAfterCaretCommand::EraseAfterCaretCommand(CWnd* parent, BOOL onChar)
	:Command(parent) {
	this->character[0] = '\0';
	this->character[1] = '\0';
	this->onChar = onChar;
	this->offset = -1;
	this->columnIndex = -1;
}

EraseAfterCaretCommand::~EraseAfterCaretCommand() {

}

EraseAfterCaretCommand::EraseAfterCaretCommand(const EraseAfterCaretCommand& source)
	:Command(source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;
}

EraseAfterCaretCommand& EraseAfterCaretCommand::operator=(const EraseAfterCaretCommand& source) {
	Command::operator=(source);

	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;

	return *this;
}

void EraseAfterCaretCommand::Execute() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 삭제할 대상이 있으면,
	if (columnIndex < row->GetLength() || rowIndex < note->GetLength() - 1) 
	{
		//2.1. 칸 위치가 줄의 끝이 아니면,
		NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
		Long vScrollChanged = 0;
		Long bytes = 0;
		if (columnIndex < row->GetLength())
		{
			//2.1.1. 노트에서 삭제한다.
			TCHAR(*letter) = (char*)*row->GetAt(columnIndex);
			this->character[0] = letter[0];
			bytes = 1;
			if (!ByteChecker::IsASCII(this->character))
			{
				this->character[1] = letter[1];
				bytes++;
			}

			row->Remove(columnIndex);
			columnIndex = row->Move(columnIndex);
			noteWidthCache->MarkDirty(rowIndex);
		}
		else //2.2. 칸 위치가 줄의 끝이면,
		{
			//2.2.1. 적재범위를 넘어서면, 재적재한다.
			if (note->IsBelowBottomLine(rowIndex+1) && !note->IsLastPage())
			{
				PageManager::LoadNext(this->parent);
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
			}

			//2.2.2. 다음줄이 진짜줄이면, 줄을 합친다.
			Glyph* nextRow = note->GetAt(rowIndex + 1);
			if (!nextRow->IsDummyRow())
			{
				this->character[0] = '\r';
				this->character[1] = '\n';
				bytes = 2;

				noteWidthCache->Remove(rowIndex + 1);
				note->MergeRows(rowIndex);
				row = note->GetAt(rowIndex);
				columnIndex = row->Move(columnIndex);
				noteWidthCache->MarkDirty(rowIndex);
				vScrollChanged = -1;
			}
			else //2.2.3. 다음줄이 가짜줄이면, 다음 줄의 첫 문자를 지운다.
			{
				TCHAR(*letter) = (char*)*nextRow->GetAt(0);
				this->character[0] = letter[0];
				bytes = 1;
				if (!ByteChecker::IsASCII(this->character))
				{
					this->character[1] = letter[1];
					bytes++;
				}

				nextRow->Remove(0);
				noteWidthCache->MarkDirty(rowIndex + 1);
			}
		}

		//2.2. 페이징 버퍼에서 지운다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (this->onChar && bytes > 0)
		{
			Long currentOffset = pagingBuffer->GetCurrentOffset();
			pagingBuffer->Remove(currentOffset + bytes);
			this->isUndoable = true;
		}

		//2.3. 자동개행중이면, 재개행한다.
		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			NoteWrapper noteWrapper(this->parent);
			vScrollChanged += noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();
		}

		//2.4. 스크롤이 있으면, 적용한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}

		this->offset = pagingBuffer->GetCurrentOffset();
		this->columnIndex = columnIndex;
	}
}

void EraseAfterCaretCommand::Undo() {
	//1. 오프셋 기반으로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 일반 문자이면,
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	Long vScrollChanged = 0;
	if (this->character[0] != '\r')
	{
		//3.1. 노트에서 삽입한다.
		Glyph* glyph = GlyphFactory::Create(this->character);
		row->Add(columnIndex, glyph);
		columnIndex = row->Move(columnIndex);
		noteWidthCache->MarkDirty(rowIndex);
	}
	else //4. 줄바꿈 문자이면,
	{
		//4.1. 노트에서 줄을 나눈다.
		note->SplitRows(rowIndex, columnIndex);
		noteWidthCache->Add(rowIndex + 1);

		rowIndex = note->Move(rowIndex);
		row = note->GetAt(rowIndex);
		columnIndex = row->Move(columnIndex);

		noteWidthCache->MarkDirty(rowIndex);
		noteWidthCache->MarkDirty(rowIndex + 1);

		vScrollChanged = 1;
	}

	//5. 페이징 버퍼에서 쓴다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Add(this->character);
	pagingBuffer->MoveOffset(this->offset);

	//6. 자동개행중이면, 재개행한다.
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
	{
		NoteWrapper noteWrapper(this->parent);
		vScrollChanged += noteWrapper.Rewrap();
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
	}

	//7. 스크롤바가 있으면 조정한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}

	this->columnIndex = columnIndex;
}

void EraseAfterCaretCommand::Redo() {
	//1. 오프셋 기반으로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 칸 위치가 줄의 끝이 아니면,
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	Long vScrollChanged = 0;
	Long bytes = 0;
	if (columnIndex < row->GetLength())
	{
		//3.1. 노트에서 삭제한다.
		bytes = 1;
		if (!ByteChecker::IsASCII(this->character))
		{
			bytes++;
		}

		row->Remove(columnIndex);
		columnIndex = row->Move(columnIndex);
		noteWidthCache->MarkDirty(rowIndex);
	}
	else //4. 칸 위치가 줄의 끝이면,
	{
		//4.1. 적재범위를 넘어서면 재적재한다.
		if (note->IsBelowBottomLine(rowIndex + 1) && !note->IsLastPage())
		{
			PageManager::LoadNext(this->parent);
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
		}

		//4.2. 다음줄이 진짜줄이면, 줄을 합친다.
		Glyph* nextRow = note->GetAt(rowIndex + 1);
		if (!nextRow->IsDummyRow())
		{
			bytes = 2;

			noteWidthCache->Remove(rowIndex + 1);
			note->MergeRows(rowIndex);
			row = note->GetAt(rowIndex);
			columnIndex = row->Move(columnIndex);
			noteWidthCache->MarkDirty(rowIndex);
			vScrollChanged = -1;
		}
		else //4.3. 다음줄이 가짜줄이면, 다음 줄의 첫 문자를 지운다.
		{
			bytes = 1;
			if (!ByteChecker::IsASCII(this->character))
			{
				bytes++;
			}

			nextRow->Remove(0);
			noteWidthCache->MarkDirty(rowIndex + 1);
		}
	}

	//5. 페이징 버퍼에서 지운다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (bytes > 0)
	{
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		pagingBuffer->Remove(currentOffset + bytes);
	}

	//6. 자동개행중이면, 재개행한다.
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
	{
		NoteWrapper noteWrapper(this->parent);
		vScrollChanged += noteWrapper.Rewrap();
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
	}

	//7. 스크롤이 있으면, 적용한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}

	this->columnIndex = columnIndex;
}

Command* EraseAfterCaretCommand::Clone() {
	return new EraseAfterCaretCommand(*this);
}
