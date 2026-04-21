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
	this->offset = -1;
	this->columnIndex = -1;

	//2. 삭제할 대상이 있으면, 지운다.
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	BOOL hasTarget = FALSE;
	Long vScrollChanged = 0;
	if (columnIndex < row->GetLength())
	{
		hasTarget = TRUE;

		Glyph* letter = row->GetAt(columnIndex);
		this->character[0] = ((char*)*letter)[0];
		if (letter->IsMultiByteCharacter())
		{
			this->character[1] = ((char*)*letter)[1];
		}
		else
		{
			this->character[1] = '\0';
		}

		row->Remove(columnIndex);
		columnIndex = row->Move(columnIndex);
		noteWidthCache->MarkDirty(rowIndex);
	}
	else if (rowIndex + 1 < note->GetLength())
	{
		Glyph* nextRow = note->GetAt(rowIndex + 1);
		if (!nextRow->IsDummyRow())
		{
			hasTarget = TRUE;
			this->character[0] = '\r';
			this->character[1] = '\n';

			noteWidthCache->Remove(rowIndex + 1);
			note->MergeRows(rowIndex);
			row = note->GetAt(rowIndex);
			columnIndex = row->Move(columnIndex);
			noteWidthCache->MarkDirty(rowIndex);
			vScrollChanged = -1;
		}
		else
		{
			hasTarget = TRUE;
			Glyph* letter = nextRow->GetAt(0);
			this->character[0] = ((char*)*letter)[0];
			if (letter->IsMultiByteCharacter())
			{
				this->character[1] = ((char*)*letter)[1];
			}
			else
			{
				this->character[1] = '\0';
			}

			nextRow->Remove(0);
			noteWidthCache->MarkDirty(rowIndex + 1);
			columnIndex = row->Move(columnIndex);
		}
	}

	//3. 자동개행중이라면, 재개행한다.
	if (hasTarget && ((NotepadForm*)(this->parent))->IsAutoWrapped())
	{
		NoteWrapper noteWrapper(this->parent);
		vScrollChanged += noteWrapper.Rewrap();
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
	}

	//4. 페이징버퍼에서 지운다.
	if (hasTarget && this->onChar)
	{
		Long originalOffset = pagingBuffer->GetCurrentOffset();
		if (this->character[0] == '\r')
		{
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
		}
		else
		{
			pagingBuffer->Next();
			pagingBuffer->Remove();
		}
		pagingBuffer->MoveOffset(originalOffset);
		this->isUndoable = TRUE;
	}

	//5. 스크롤이 있으면, 적용한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (hasTarget && scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}

	if (hasTarget)
	{
		this->offset = pagingBuffer->GetCurrentOffset();
		this->columnIndex = columnIndex;
	}
}

void EraseAfterCaretCommand::Undo() {
	//1. 오프셋이 존재하면,
	if (this->offset >= 0)
	{
		//1.1. 오프셋 기반으로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(this->offset);
		caretNavigator.NormalizeColumn(this->columnIndex);

		//1.2. 현재 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
		Long vScrollChanged = 0;
		if (this->character[0] != '\r')
		{
			GlyphFactory glyphFactory;
			Glyph* glyph = glyphFactory.Create(this->character);
			row->Add(columnIndex, glyph);
			columnIndex = row->Move(columnIndex);
			noteWidthCache->MarkDirty(rowIndex);
		}
		else
		{
			note->SplitRows(rowIndex, columnIndex);
			noteWidthCache->Add(rowIndex + 1);

			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			columnIndex = row->Move(columnIndex);
	
			noteWidthCache->MarkDirty(rowIndex);
			noteWidthCache->MarkDirty(rowIndex + 1);

			vScrollChanged = 1;
		}

		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			NoteWrapper noteWrapper(this->parent);
			vScrollChanged += noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();
		}

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		pagingBuffer->Add(this->character);
		pagingBuffer->MoveOffset(this->offset);

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
}

void EraseAfterCaretCommand::Redo() {
	//1. 오프셋이 존재하면,
	if (this->offset >= 0)
	{
		//1.1. 오프셋 기반으로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(this->offset);
		caretNavigator.NormalizeColumn(this->columnIndex);

		//1.2. 현재 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//1.3. 지울 대상을 다시 지운다.
		NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
		Long vScrollChanged = 0;
		if (this->character[0] != '\r')
		{
			if (columnIndex < row->GetLength())
			{
				row->Remove(columnIndex);
				columnIndex = row->Move(columnIndex);
				noteWidthCache->MarkDirty(rowIndex);
			}
			else if (rowIndex + 1 < note->GetLength())
			{
				Glyph* nextRow = note->GetAt(rowIndex + 1);
				if (nextRow->GetLength() > 0)
				{
					nextRow->Remove(0);
					columnIndex = row->Move(columnIndex);
					noteWidthCache->MarkDirty(rowIndex + 1);
				}
			}
		}
		else if (rowIndex + 1 < note->GetLength())
		{
			note->MergeRows(rowIndex);
			noteWidthCache->Remove(rowIndex + 1);

			row = note->GetAt(rowIndex);
			columnIndex = row->Move(columnIndex);
			noteWidthCache->MarkDirty(rowIndex);

			vScrollChanged = -1;
		}

		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			NoteWrapper noteWrapper(this->parent);
			vScrollChanged += noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();
		}

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (this->character[0] == '\r')
		{
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
		}
		else
		{
			pagingBuffer->Next();
			pagingBuffer->Remove();
		}
		pagingBuffer->MoveOffset(this->offset);

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
}

Command* EraseAfterCaretCommand::Clone() {
	return new EraseAfterCaretCommand(*this);
}