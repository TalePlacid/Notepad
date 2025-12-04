#include "InsertAtCaretCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "PagingBuffer.h"
#include "resource.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "pagingNavigator.h"

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
	GlyphFactory glyphFactory;
	Glyph* glyph;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(columnIndex - 1);
		columnIndex = row->GetCurrent();
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (this->character[0] != '\r')
	{
		glyph = glyphFactory.Create(this->character);
		row->Add(columnIndex, glyph);
		if (this->onChar)
		{
			pagingBuffer->Add(this->character);
		}
	}
	else
	{
		if (columnIndex < row->GetLength())
		{
			note->SplitRows(rowIndex, columnIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}
		else
		{
			glyph = glyphFactory.Create(this->character);
			rowIndex = note->Add(rowIndex + 1, glyph);
		}
		pagingBuffer->Add(this->character);

		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();
}

void InsertAtCaretCommand::Undo() {
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	if (columnIndex > 0)
	{
		row->Remove(columnIndex - 1);
	}
	else
	{
		if (note->IsAboveTopLine(rowIndex - 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = note->GetCurrent();
		}
		if (rowIndex > 0)
		{
			Glyph* previousRow = note->GetAt(rowIndex - 1);
			columnIndex = previousRow->Last();
			rowIndex = note->MergeRows(rowIndex - 1);
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			columnIndex = row->Move(columnIndex);

			ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
			SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() - sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Remove();

	this->offset = pagingBuffer->GetCurrentOffset();
}

void InsertAtCaretCommand::Redo() {
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	GlyphFactory glyphFactory;
	Glyph* glyph;
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(columnIndex - 1);
	}

	if (this->character[0] != '\r')
	{
		glyph = glyphFactory.Create(this->character);
		row->Add(columnIndex, glyph);
	}
	else
	{
		if (columnIndex < row->GetLength())
		{
			note->SplitRows(rowIndex, columnIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}
		else
		{
			glyph = glyphFactory.Create(this->character);
			rowIndex = note->Add(rowIndex + 1, glyph);
		}

		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Add(this->character);
	
	this->offset = pagingBuffer->GetCurrentOffset();
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
