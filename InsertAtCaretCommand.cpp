#include "InsertAtCaretCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "PagingBuffer.h"
#include "resource.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "pagingNavigator.h"
#include "NoteWrapper.h"

#pragma warning(disable:4996)

InsertAtCaretCommand::InsertAtCaretCommand(CWnd* parent, char(*character), BOOL onChar)
	:Command(parent) {
	this->character[0] = character[0];
	if (character[0] & 0x80 || character[0] == '\r')
	{
		this->character[1] = character[1];
	}
	this->onChar = onChar;
	this->offset = -1;
	this->columnIndex = -1;
}

InsertAtCaretCommand::~InsertAtCaretCommand() {

}

InsertAtCaretCommand::InsertAtCaretCommand(const InsertAtCaretCommand& source)
	:Command(source) {
	this->character[0] = const_cast<InsertAtCaretCommand&>(source).character[0];
	if (this->character[0] & 0x80)
	{
		this->character[1] = const_cast<InsertAtCaretCommand&>(source).character[1];
	}
	this->onChar = source.onChar;
	this->offset = source.offset;
}

InsertAtCaretCommand& InsertAtCaretCommand::operator=(const InsertAtCaretCommand& source){
	Command::operator=(source);

	this->character[0] = const_cast<InsertAtCaretCommand&>(source).character[0];
	if (this->character[0] & 0x80)
	{
		this->character[1] = const_cast<InsertAtCaretCommand&>(source).character[1];
	}
	this->onChar = source.onChar;
	this->offset = source.offset;

	return *this;
}

void InsertAtCaretCommand::Execute() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 조합중이면, 조합중이던 글자를 지운다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(columnIndex - 1);
		columnIndex = row->GetCurrent();
	}

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (this->character[0] != '\r') //3. 줄바꿈 문자가 아니면,
	{
		//3.1. 노트에서 더한다.
		GlyphFactory glyphFactory;
		Glyph* glyph = glyphFactory.Create(this->character);
		columnIndex = row->Add(columnIndex, glyph);
		columnIndex = row->GetCurrent();

		//3.2. 자동개행이 켜져 있다면,
		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			//3.2.1. 재개행한다.
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();

			//3.2.2. 수직 스크롤이 있다면, 반영한다.
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}

		//3.3. 조합 확정이면, 페이징버퍼에서 적는다. 
		if (this->onChar)
		{
			pagingBuffer->Add(this->character);
		}
	}
	else //4. 줄바꿈 문자라면,
	{
		//4.1. 줄을 나눈다.
		note->SplitRows(rowIndex, columnIndex);
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
	
		//4.2. 페이징버퍼에서 적는다.
		pagingBuffer->Add(this->character);

		//4.3. 수직 스크롤이 있다면, 반영한다.
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}

	this->offset = pagingBuffer->GetCurrentOffset();
	this->columnIndex = columnIndex;
}

void InsertAtCaretCommand::Undo() {
	//1. 오프셋 기반으로 이동한다.
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);
	pagingNavigator.NormalizeColumn(this->columnIndex);

	//2. 현재 위치룰 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	NoteWrapper noteWrapper(this->parent);
	Long dummied;
	if (columnIndex > 0) //3. 줄의 처음이 아니면,
	{
		//3.1. 노트에서 지운다.
		row->Remove(columnIndex - 1);
		columnIndex = row->GetCurrent();

		//3.2. 자동개행중이라면,
		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			//3.2.1. 재개행한다.
			dummied = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();

			//3.2.2. 수직 스크롤이 있다면, 반영한다.
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
	}
	else //4. 줄의 처음이라면,
	{
		//4.1. 적재범위에서 벗어났다면, 재적재한다.
		if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}

		//4.2. 첫번째 줄이 아니라면,
		if (rowIndex > 0)
		{
			//4.2.1. 이전 줄과 합친다.
			Glyph* previousRow = note->GetAt(rowIndex - 1);
			columnIndex = previousRow->Last();

			rowIndex = note->MergeRows(rowIndex - 1);
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			columnIndex = row->Move(columnIndex);

			//4.2.2. 자동개행중이라면, 재개행한다.
			Long vScrollChanged = 0;
			if (((NotepadForm*)(this->parent))->isAutoWrapped)
			{
				//4.2.2.1. 재개행한다.
				vScrollChanged = noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();				
			}
			else //4.2.3. 자동개행중이 아니라면,
			{
				vScrollChanged = -1;				
			}
			
			//4.2.3. 수직 스크롤이 있다면, 반영한다.
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
	}

	//5. 페이징 버퍼에서 지운다.
	pagingBuffer->Remove();

	this->offset = pagingBuffer->GetCurrentOffset();
	this->columnIndex = columnIndex;
}

void InsertAtCaretCommand::Redo() {
	//1. 오프셋 기반으로 이동한다.
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);
	pagingNavigator.NormalizeColumn(this->columnIndex);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (this->character[0] != '\r') //3. 줄바꿈 문자가 아니면,
	{
		//3.1. 노트에서 더한다.
		GlyphFactory glyphFactory;
		Glyph* glyph = glyphFactory.Create(this->character);
		columnIndex = row->Add(columnIndex, glyph);
		columnIndex = row->GetCurrent();

		//3.2. 자동개행이 켜져 있다면,
		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			//3.2.1. 재개행한다.
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();

			//3.2.2. 수직 스크롤이 있다면, 반영한다.
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
	}
	else //4. 줄바꿈 문자라면,
	{
		//4.1. 줄을 나눈다.
		note->SplitRows(rowIndex, columnIndex);
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();

		//4.3. 수직 스크롤이 있다면, 반영한다.
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}

	//5. 페이징버퍼에서 적는다.
	pagingBuffer->Add(this->character);

	this->offset = pagingBuffer->GetCurrentOffset();
	this->columnIndex = columnIndex;
}

Command* InsertAtCaretCommand::Clone() {
	return new InsertAtCaretCommand(*this);
}

UINT InsertAtCaretCommand::GetId() {
	return ID_COMMAND_INSERTATCARET;
}

bool InsertAtCaretCommand::IsUndoable() {
	return true;
}
