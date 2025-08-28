#include "InsertAtCaretCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

InsertAtCaretCommand::InsertAtCaretCommand(CWnd* parent, char(*character), BOOL onChar)
	:UndoableCommand(parent) {
	this->character[0] = character[0];
	if (character[0] & 0x80)
	{
		this->character[1] = character[1];
	}
	this->onChar = onChar;
	this->offset = -1;
}

InsertAtCaretCommand::~InsertAtCaretCommand() {

}

InsertAtCaretCommand::InsertAtCaretCommand(const InsertAtCaretCommand& source)
	:UndoableCommand(source) {
	this->character[0] = const_cast<InsertAtCaretCommand&>(source).character[0];
	if (this->character[0] & 0x80)
	{
		this->character[1] = const_cast<InsertAtCaretCommand&>(source).character[1];
	}
	this->offset = source.offset;
}

InsertAtCaretCommand& InsertAtCaretCommand::operator=(const InsertAtCaretCommand& source){
	UndoableCommand::operator=(source);

	this->character[0] = const_cast<InsertAtCaretCommand&>(source).character[0];
	if (this->character[0] & 0x80)
	{
		this->character[1] = const_cast<InsertAtCaretCommand&>(source).character[1];
	}
	this->offset = source.offset;

	return *this;
}

void InsertAtCaretCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();

	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(columnIndex - 1);
	}

	if (this->character[0] != '\r')
	{
		row->Add(row->GetCurrent(), glyph);
		if (this->onChar)
		{
			pagingBuffer->Add(this->character);
		}
	}
	else
	{
		this->character[0] = '\r';
		this->character[1] = '\n';

		if (this->onChar)
		{
			pagingBuffer->Add(this->character);
		}

		rowIndex = note->SplitRows(rowIndex, columnIndex);
		rowIndex = note->Move(rowIndex);
		row = note->GetAt(rowIndex);
		row->First();

		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();
}

void InsertAtCaretCommand::Undo() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MoveOffset(this->offset);
	if (!pagingBuffer->IsOnPage(this->offset))
	{
		pagingBuffer->Load();
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->Move(pagingBuffer->GetCurrent().GetColumn());

	if (columnIndex > 0)
	{
		row->Remove(columnIndex - 1);
	}
	else
	{
		if (rowIndex > 0)
		{
			Glyph* previousRow = note->GetAt(rowIndex - 1);
			Long  previousLastIndex = previousRow->GetLength();
			rowIndex = note->MergeRows(rowIndex - 1);
			rowIndex = note->Move(rowIndex);
			row = note->GetAt(rowIndex);
			row->Move(previousLastIndex);
		}
	}

	pagingBuffer->Remove();
}


Command* InsertAtCaretCommand::Clone() {
	return new InsertAtCaretCommand(*this);
}
