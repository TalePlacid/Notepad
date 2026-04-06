#include "InsertAtCaretCommand.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
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
	this->columnIndex = -1;
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
	Editor editor(this->parent);
	this->isErased = editor.GetSelectedRange(this->erasedFrontOffset, this->erasedRearOffset);
	if (this->isErased == TRUE)
	{
		this->isUndoable = true;
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		this->erasedContents = pagingBuffer->MakeSelectedString();
		editor.EraseRange(this->erasedFrontOffset, this->erasedRearOffset, this->erasedContents, this->erasedColumnIndex);
	}

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 조합중이면, 조합중이던 글자를 지운다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(columnIndex - 1);
		columnIndex = row->GetCurrent();
	}

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long vScrollChanged = 0;
	if (this->character[0] != '\r') //4. 줄바꿈 문자가 아니면,
	{
		//4.1. 노트에서 더한다.
		GlyphFactory glyphFactory;
		Glyph* glyph = glyphFactory.Create(this->character);
		columnIndex = row->Add(columnIndex, glyph);
		columnIndex = row->GetCurrent();

		//4.2. 조합 확정이면, 페이징버퍼에서 적는다. 
		if (this->onChar)
		{
			pagingBuffer->Add(this->character);
			this->isUndoable = TRUE;
		}
	}
	else //5. 줄바꿈 문자라면,
	{
		//5.1. 줄을 나눈다.
		note->SplitRows(rowIndex, columnIndex);

		//5.2. 자동개행이 켜져 있다면,
		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			//5.2.1. 재개행한다.
			NoteWrapper noteWrapper(this->parent);
			vScrollChanged = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();
		}

		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
	
		//5.3. 페이징버퍼에서 적는다.
		pagingBuffer->Add(this->character);
		this->isUndoable = TRUE;

		vScrollChanged = 1;
	}

	//6. 자동개행이 켜져 있다면,
	if (((NotepadForm*)(this->parent))->IsAutoWrapped())
	{
		//6.1. 재개행한다.
		NoteWrapper noteWrapper(this->parent);
		vScrollChanged = noteWrapper.Rewrap();
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		columnIndex = row->GetCurrent();
	}

	//7. 수직 스크롤이 있다면, 반영한다.
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}

	this->offset = pagingBuffer->GetCurrentOffset();
	this->columnIndex = columnIndex;
}

void InsertAtCaretCommand::Undo() {
	//1. 오프셋 기반으로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. 확정문자였었으면,
	if (this->onChar)
	{
		//2.1. 현재 위치룰 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		NoteWrapper noteWrapper(this->parent);
		Long vScrollChanged = 0;
		if (columnIndex > 0) //2.2. 줄의 처음이 아니면,
		{
			//2.2.1. 노트에서 지운다.
			row->Remove(columnIndex - 1);
			columnIndex = row->GetCurrent();

			//2.2.2. 자동개행중이라면,
			if (((NotepadForm*)(this->parent))->IsAutoWrapped())
			{
				//2.2.2.1. 재개행한다.
				vScrollChanged = noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
		}
		else //2.3. 줄의 처음이라면,
		{
			//2.3.1. 적재범위에서 벗어났다면, 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				PageManager::LoadPrevious(this->parent);
				rowIndex = note->GetCurrent();
			}

			//2.3.2. 첫번째 줄이 아니라면,
			if (rowIndex > 0)
			{
				//2.3.2.1. 이전 줄과 합친다.
				Glyph* previousRow = note->GetAt(rowIndex - 1);
				columnIndex = previousRow->Last();

				rowIndex = note->MergeRows(rowIndex - 1);
				rowIndex = note->Move(rowIndex);
				row = note->GetAt(rowIndex);
				columnIndex = row->Move(columnIndex);

				//2.3.2.2. 자동개행중이라면, 재개행한다.
				if (((NotepadForm*)(this->parent))->IsAutoWrapped())
				{
					//4.2.2.1. 재개행한다.
					vScrollChanged = noteWrapper.Rewrap();
					rowIndex = note->GetCurrent();
					row = note->GetAt(rowIndex);
					columnIndex = row->GetCurrent();
				}
				else //2.3.2.3. 자동개행중이 아니라면,
				{
					vScrollChanged = -1;
				}
			}
		}

		//2.4. 페이징 버퍼에서 지운다.
		pagingBuffer->Remove();

		//2.5. 수직 스크롤이 있다면, 반영한다.
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}

		this->offset = pagingBuffer->GetCurrentOffset();
		this->columnIndex = columnIndex;
	}

	//3. 범위가 지워졌었으면, 다시 적는다.
	if (this->isErased)
	{
		Editor editor(this->parent);
		editor.InsertTextAt(this->erasedFrontOffset, this->erasedColumnIndex, this->erasedContents, true);
	}
}

void InsertAtCaretCommand::Redo() {
	//1. 선택범위가 지워졌었으면, 다시 지운다.
	if (this->isErased)
	{
		Editor editor(this->parent);
		editor.EraseRange(this->erasedFrontOffset, this->erasedRearOffset, this->erasedContents, this->erasedColumnIndex);
	}

	//2. 확정문자였었으면, 다시 적는다.
	if (this->onChar)
	{
		//2.1. 오프셋 기반으로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(this->offset);
		caretNavigator.NormalizeColumn(this->columnIndex);

		//2.2. 현재 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long vScrollChanged = 0;
		if (this->character[0] != '\r') //2.3. 줄바꿈 문자가 아니면,
		{
			//2.3.1. 노트에서 더한다.
			GlyphFactory glyphFactory;
			Glyph* glyph = glyphFactory.Create(this->character, true);
			columnIndex = row->Add(columnIndex, glyph);
			columnIndex = row->GetCurrent();
		}
		else //2.4. 줄바꿈 문자라면,
		{
			//2.4.1. 줄을 나눈다.
			note->SplitRows(rowIndex, columnIndex);
			
			//2.4.2. 자동개행이 켜져 있다면,
			if (((NotepadForm*)(this->parent))->IsAutoWrapped())
			{
				//2.4.2.1. 재개행한다.
				NoteWrapper noteWrapper(this->parent);
				vScrollChanged = noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}

			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			vScrollChanged = 1;
		}

		//2.5. 자동개행이 켜져 있다면,
		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			//2.5.1. 재개행한다.
			NoteWrapper noteWrapper(this->parent);
			vScrollChanged = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();
		}

		//2.6. 페이징버퍼에서 적는다.
		pagingBuffer->Add(this->character);

		//2.7. 수직 스크롤이 있다면, 반영한다.
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

Command* InsertAtCaretCommand::Clone() {
	return new InsertAtCaretCommand(*this);
}
