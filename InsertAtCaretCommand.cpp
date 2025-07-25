#include <afxwin.h>
#include "InsertAtCaretCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "PagingBuffer.h"

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
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();

	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove(columnIndex - 1);
		pagingBuffer->Remove();
	}

	if (this->character[0] != '\r')
	{
		row->Add(row->GetCurrent(), glyph);
		pagingBuffer->Add((char*)(*glyph));
	}
	else
	{
		rowIndex = note->Add(rowIndex + 1, glyph);
		TCHAR contents[2];
		contents[0] = '\r';
		contents[1] = '\n';
		Long ret = pagingBuffer->Add(contents);

		Glyph* clone;
		Glyph* newRow = note->GetAt(rowIndex);
		Long i = columnIndex;
		while (i < row->GetLength())
		{
			clone = row->GetAt(i)->Clone();
			newRow->Add(clone);
			i++;
		}

		i = row->GetLength() - 1;
		while (i >= columnIndex)
		{
			row->Remove(i);
			i--;
		}

		newRow->First();
	}
}

