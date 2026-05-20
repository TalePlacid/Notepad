#include "InsertAtCaretCommand.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../glyphs/NoteWidthCache.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../caretNavigator.h"
#include "../NoteWrapper.h"
#include "../PageManager.h"
#include "../Editor.h"

#pragma warning(disable:4996)

InsertAtCaretCommand::InsertAtCaretCommand(CWnd* parent, const TCHAR(*character), BOOL onChar)
	:Command(parent) {
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->onChar = onChar;
	this->offset = -1;
	this->columnIndex = 0;
	this->isErased = FALSE;
	this->erasedFrontOffset = -1;
	this->erasedRearOffset = -1;
	this->erasedColumnIndex = 0;
	this->erasedContents = "";
}

InsertAtCaretCommand::~InsertAtCaretCommand() {

}

InsertAtCaretCommand::InsertAtCaretCommand(const InsertAtCaretCommand& source)
	:Command(source) {
	this->character[0] = const_cast<InsertAtCaretCommand&>(source).character[0];
	this->character[1] = const_cast<InsertAtCaretCommand&>(source).character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;
	this->isErased = source.isErased;
	this->erasedFrontOffset = source.erasedFrontOffset;
	this->erasedRearOffset = source.erasedRearOffset;
	this->erasedColumnIndex = source.erasedColumnIndex;
	this->erasedContents = source.erasedContents;
}

InsertAtCaretCommand& InsertAtCaretCommand::operator=(const InsertAtCaretCommand& source){
	Command::operator=(source);

	this->character[0] = const_cast<InsertAtCaretCommand&>(source).character[0];
	this->character[1] = const_cast<InsertAtCaretCommand&>(source).character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;
	this->isErased = source.isErased;
	this->erasedFrontOffset = source.erasedFrontOffset;
	this->erasedRearOffset = source.erasedRearOffset;
	this->erasedColumnIndex = source.erasedColumnIndex;
	this->erasedContents = source.erasedContents;

	return *this;
}

void InsertAtCaretCommand::Execute() {
	//1. 선택범위가 있으면, 먼저 지운다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Editor editor(this->parent);
	this->isErased = editor.GetSelectedRange(this->erasedFrontOffset, this->erasedRearOffset);
	if (this->isErased == TRUE)
	{
		this->isUndoable = true;
		this->erasedContents = pagingBuffer->MakeSelectedString();
		editor.EraseRange(this->erasedFrontOffset, this->erasedRearOffset, this->erasedContents, this->erasedColumnIndex);
	}

	//2. 노트에서 조합중인 문자가 있으면, 지운다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 조합문자가 남아 있으면, 조합중이던 글자를 지운다.
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	BOOL hasCompositionCharacter = ((NotepadForm*)(this->parent))->HasCompositionCharacter();
	if (hasCompositionCharacter)
	{
		row->Remove(columnIndex - 1);
		noteWidthCache->MarkDirty(rowIndex);
		columnIndex = row->GetCurrent();
		((NotepadForm*)(this->parent))->UnmarkCompositionCharacterExist();
	}

	//3. 노트에서 적는다.
	Long vScrollChanged = 0;
	Glyph* glyph = GlyphFactory::Create(this->character);
	if (this->character[0] != '\r')
	{
		row->Add(columnIndex, glyph);
		columnIndex = row->GetCurrent();
		noteWidthCache->MarkDirty(rowIndex);
	}
	else
	{
		note->SplitRows(rowIndex, columnIndex);
		noteWidthCache->Add(rowIndex + 1);
		noteWidthCache->MarkDirty(rowIndex);

		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		noteWidthCache->MarkDirty(rowIndex);

		vScrollChanged++;
	}

	//4. 확정문자이면, 페이징버퍼에서 적는다.
	if (this->onChar)
	{
		TCHAR character_[3] = { this->character[0], this->character[1], '\0' };
		pagingBuffer->Add(character_);
		this->isUndoable = TRUE;
	}
	else
	{
		((NotepadForm*)(this->parent))->MarkCompositionCharacterExist();
	}

	//5. 자동개행중이면, 재개행한다.
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
	{
		NoteWrapper noteWrapper(this->parent);
		vScrollChanged += noteWrapper.Rewrap();
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
	}

	//6. 수직 스크롤이 있으면 변화량을 적용한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long max = vScroll.GetMax() + vScrollChanged * rowHeight;
		scrollController->ResizeVRange(max);
	}

	this->offset = pagingBuffer->GetCurrentOffset();
	this->columnIndex = columnIndex;
}

void InsertAtCaretCommand::Undo() {
	//1. 확정문자였으면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Long vScrollChanged = 0;
	if (this->onChar)
	{
		//1.1. 오프셋 기반으로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(this->offset);
		caretNavigator.NormalizeColumn(this->columnIndex);

		//1.2. 노트에서 지운다.
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();

		NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		NoteWrapper noteWrapper(this->parent);
		if (this->character[0] != '\r')
		{
			row->Remove(columnIndex - 1);
			columnIndex = row->GetCurrent();
			noteWidthCache->MarkDirty(rowIndex);

			if (((NotepadForm*)(this->parent))->IsAutoWrapped())
			{
				vScrollChanged += noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
		}
		else
		{
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				PageManager::LoadPrevious(this->parent);
				rowIndex = note->GetCurrent();
			}

			if (rowIndex > 0)
			{
				Glyph* previousRow = note->GetAt(rowIndex - 1);
				columnIndex = previousRow->Last();

				rowIndex = note->MergeRows(rowIndex - 1);
				noteWidthCache->Remove(rowIndex + 1);

				rowIndex = note->Move(rowIndex);
				row = note->GetAt(rowIndex);
				columnIndex = row->Move(columnIndex);
				noteWidthCache->MarkDirty(rowIndex);

				if (((NotepadForm*)(this->parent))->IsAutoWrapped())
				{
					vScrollChanged += noteWrapper.Rewrap();
					rowIndex = note->GetCurrent();
					row = note->GetAt(rowIndex);
					columnIndex = row->GetCurrent();
				}
				else
				{
					vScrollChanged -= 1;
				}
			}
		}

		//1.3. 페이징 버퍼에서 지운다.
		pagingBuffer->Remove();

		//1.4. 수직 스크롤이 있다면 변화량을 반영한다.
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long rowHeight = sizeCalculator->GetRowHeight();
			Long max = vScroll.GetMax() + vScrollChanged * rowHeight;
			scrollController->ResizeVRange(max);
		}
	}

	//2. 범위 삭제 되었었으면, 다시 적는다.
	if (this->isErased)
	{
		Editor editor(this->parent);
		editor.InsertTextAt(this->erasedFrontOffset, this->erasedColumnIndex, this->erasedContents, true);
	}

	this->offset = pagingBuffer->GetCurrentOffset();
	this->columnIndex = columnIndex;
}

void InsertAtCaretCommand::Redo() {
	//1. 오프셋 기반으로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. 선택범위가 있으면, 먼저 지운다.
	Editor editor(this->parent);
	if (this->isErased == TRUE)
	{
		editor.EraseRange(this->erasedFrontOffset, this->erasedRearOffset, this->erasedContents, this->erasedColumnIndex);
	}

	//3. 확정문자였었으면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Long vScrollChanged = 0;
	if (this->onChar)
	{
		//3.1. 노트에서 적는다.
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();

		NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
		Glyph* glyph = GlyphFactory::Create(this->character);
		if (this->character[0] != '\r')
		{
			row->Add(columnIndex, glyph);
			columnIndex = row->GetCurrent();
			noteWidthCache->MarkDirty(rowIndex);
		}
		else
		{
			note->SplitRows(rowIndex, columnIndex);
			noteWidthCache->Add(rowIndex + 1);
			noteWidthCache->MarkDirty(rowIndex);

			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			noteWidthCache->MarkDirty(rowIndex);

			vScrollChanged++;
		}

		//3.2. 페이징버퍼에서 적는다.
		TCHAR character_[3] = { this->character[0], this->character[1], '\0' };
		pagingBuffer->Add(character_);
	}

	//4. 자동개행중이면 재개행한다.
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
	{
		NoteWrapper noteWrapper(this->parent);
		vScrollChanged += noteWrapper.Rewrap();
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
	}

	//5. 수직 스크롤이 있으면 변화량을 적용한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long max = vScroll.GetMax() + vScrollChanged * rowHeight;
		scrollController->ResizeVRange(max);
	}

	this->offset = pagingBuffer->GetCurrentOffset();
	this->columnIndex = columnIndex;
}

Command* InsertAtCaretCommand::Clone() {
	return new InsertAtCaretCommand(*this);
}
