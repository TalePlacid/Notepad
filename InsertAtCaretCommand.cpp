#include <afxwin.h>
#include "InsertAtCaretCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"

#pragma warning(disable:4996)

InsertAtCaretCommand::InsertAtCaretCommand(CWnd* parent, char(*character))
	:Command(parent) {
	this->character[0] = character[0];
	if (character[0] & 0x80)
	{
		this->character[1] = character[1];
	}
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
}

InsertAtCaretCommand& InsertAtCaretCommand::operator=(const InsertAtCaretCommand& source){
	Command::operator=(source);

	this->character[0] = const_cast<InsertAtCaretCommand&>(source).character[0];
	if (this->character[0] & 0x80)
	{
		this->character[1] = const_cast<InsertAtCaretCommand&>(source).character[1];
	}

	return *this;
}

void InsertAtCaretCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();

	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(columnIndex);
	}

	if (this->character[0] != '\r')
	{
		row->Add(row->GetCurrent() + 1, glyph);
	}
	else
	{
		rowIndex = ((NotepadForm*)(this->parent))->note->Add(rowIndex + 1, glyph);
		Glyph* newRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

		Long i = columnIndex + 1;
		while (i < row->GetLength())
		{
			newRow->Add(row->GetAt(i)->Clone());
			i++;
		}

		i = row->GetLength() - 1;
		while (i > columnIndex)
		{
			row->Remove(i);
			i--;
		}

		newRow->First();
	}
}

