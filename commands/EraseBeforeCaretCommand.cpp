#include "EraseBeforeCaretCommand.h"
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

EraseBeforeCaretCommand::EraseBeforeCaretCommand(CWnd* parent, BOOL onChar)
	:Command(parent) {
	this->onChar = onChar;
	this->offset = -1;
	this->columnIndex = 0;
}

EraseBeforeCaretCommand::~EraseBeforeCaretCommand() {

}

EraseBeforeCaretCommand::EraseBeforeCaretCommand(const EraseBeforeCaretCommand& source)
	:Command(source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;
}

EraseBeforeCaretCommand& EraseBeforeCaretCommand::operator=(const EraseBeforeCaretCommand& source) {
	Command::operator=(source);

	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;

	return *this;
}

void EraseBeforeCaretCommand::Execute() {
	//1. 파일의 시작 위치가 아니거나, 조합중 문자가 존재하면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetCurrentOffset() > 0
		|| (!this->onChar && ((NotepadForm*)(this->parent))->HasCompositionCharacter()))
	{
		//1.1. 현재 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//1.2. 줄의 처음이 아니라면,
		NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
		Long bytes = 0;
		Long vScrollChanged = 0;
		if (columnIndex > 0)
		{
			//1.2.1. 노트에서 지운다.
			Glyph* letter = row->GetAt(columnIndex - 1);
			TCHAR(*letter_) = (char*)(*letter);
			this->character[0] = letter_[0];
			bytes = 1;
			if (letter->IsMultiByteCharacter())
			{
				this->character[1] = letter_[1];
				bytes = 2;
			}

			row->Remove(columnIndex - 1);
			columnIndex = row->GetCurrent();
			noteWidthCache->MarkDirty(rowIndex);
		}
		else //1.3. 줄의 처음이면,
		{
			//1.3.1. 적재범위를 넘어서면, 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				PageManager::LoadPrevious(this->parent);
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
			}

			//1.3.2. 현재 줄이 진짜줄이면, 이전줄과 합친다.
			if (!row->IsDummyRow())
			{
				this->character[0] = '\r';
				this->character[1] = '\n';
				bytes = 2;

				columnIndex = note->GetAt(rowIndex - 1)->GetLength();
				noteWidthCache->Remove(rowIndex);
				rowIndex = note->MergeRows(rowIndex - 1);
				row = note->GetAt(rowIndex);
				columnIndex = row->Move(columnIndex);
				noteWidthCache->MarkDirty(rowIndex);

				vScrollChanged = -1;
			}
			else //1.3.3. 현재줄이 가짜줄이면, 이전줄의 마지막 글자를 지운다.
			{
				Glyph* previousRow = note->GetAt(rowIndex - 1);
				Glyph* letter = previousRow->GetAt(previousRow->GetLength() - 1);
				TCHAR(*letter_) = (char*)(*letter);
				this->character[0] = letter_[0];
				bytes = 1;
				if (letter->IsMultiByteCharacter())
				{
					this->character[1] = letter_[1];
					bytes = 2;
				}

				previousRow->Remove();
				noteWidthCache->MarkDirty(rowIndex - 1);
			}
		}

		//1.4. 페이징버퍼에서 지운다.
		if (this->onChar && bytes > 0)
		{
			Long currentOffset = pagingBuffer->GetCurrentOffset();
			currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
			pagingBuffer->Remove(currentOffset + bytes);
			this->isUndoable = true;
		}

		//1.5. 자동개행중이면, 재개행한다.
		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			NoteWrapper noteWrapper(this->parent);
			vScrollChanged += noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();
		}

		//1.6. 스크롤이 있으면, 변화량을 적용한다.
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

void EraseBeforeCaretCommand::Undo() {
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
		GlyphFactory glyphFactory;
		Glyph* glyph = glyphFactory.Create(this->character);
		row->Add(columnIndex, glyph);
		columnIndex = row->GetCurrent();
		noteWidthCache->MarkDirty(rowIndex);
	}
	else //4. 줄바꿈 문자이면,
	{
		//4.1. 노트에서 줄을 나눈다.
		note->SplitRows(rowIndex, columnIndex);
		noteWidthCache->Add(rowIndex + 1);

		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		noteWidthCache->MarkDirty(rowIndex - 1);
		noteWidthCache->MarkDirty(rowIndex);

		vScrollChanged = 1;
	}

	//5. 페이징 버퍼에서 쓴다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->Add(this->character);

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

	this->offset = currentOffset;
	this->columnIndex = columnIndex;
}

void EraseBeforeCaretCommand::Redo() {
	//1. 오프셋 기반으로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 줄의 처음이 아니라면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	Long bytes = 0;
	Long vScrollChanged = 0;
	if (columnIndex > 0)
	{
		//3.1. 노트에서 지운다.
		bytes = 1;
		if (!ByteChecker::IsASCII(this->character))
		{
			bytes = 2;
		}

		row->Remove(columnIndex - 1);
		columnIndex = row->GetCurrent();
		noteWidthCache->MarkDirty(rowIndex);
	}
	else //4. 줄의 처음이면,
	{
		//4.1. 적재범위를 넘어서면, 재적재한다.
		if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			PageManager::LoadPrevious(this->parent);
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
		}

		//4.2. 현재 줄이 진짜줄이면, 이전줄과 합친다.
		if (!row->IsDummyRow())
		{
			bytes = 2;

			columnIndex = note->GetAt(rowIndex - 1)->GetLength();
			noteWidthCache->Remove(rowIndex);
			rowIndex = note->MergeRows(rowIndex - 1);
			row = note->GetAt(rowIndex);
			columnIndex = row->Move(columnIndex);
			noteWidthCache->MarkDirty(rowIndex);

			vScrollChanged = -1;
		}
		else //4.3. 현재줄이 가짜줄이면, 이전줄의 마지막 글자를 지운다.
		{
			Glyph* previousRow = note->GetAt(rowIndex - 1);
			bytes = 1;
			if (!ByteChecker::IsASCII(this->character))
			{
				bytes = 2;
			}

			previousRow->Remove();
			noteWidthCache->MarkDirty(rowIndex - 1);
		}
	}

	//5. 페이징버퍼에서 지운다.
	if (bytes > 0)
	{
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
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

	//7. 스크롤이 있으면, 변화량을 적용한다.
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

Command* EraseBeforeCaretCommand::Clone() {
	return new EraseBeforeCaretCommand(*this);
}

