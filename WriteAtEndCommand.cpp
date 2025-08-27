#include "WriteAtEndCommand.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

WriteAtEndCommand::WriteAtEndCommand(CWnd* parent, char(*character), BOOL onChar)
	:UndoableCommand(parent) {
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->onChar = onChar;
}

WriteAtEndCommand::~WriteAtEndCommand() {

}

WriteAtEndCommand::WriteAtEndCommand(const WriteAtEndCommand& source)
	:UndoableCommand(source.parent) {
	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];
	this->onChar = source.onChar;
}

WriteAtEndCommand& WriteAtEndCommand::operator=(const WriteAtEndCommand& source) {
	UndoableCommand::operator=(source);

	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];
	this->onChar = source.onChar;
	return *this;
}

void WriteAtEndCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Glyph* row = note->GetAt(note->GetCurrent());
	
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove();
	}

	if (this->character[0] != '\r')
	{
		row->Add(glyph);
		if (this->onChar)
		{
			pagingBuffer->Add((char*)(*glyph));
		}
	}
	else
	{
		note->Add(glyph);
		TCHAR contents[2];
		contents[0] = '\r';
		contents[1] = '\n';
		pagingBuffer->Add(contents);
		
		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
		}
	}
}

void WriteAtEndCommand::Undo() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetLength() - 1;
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetLength();

	if (columnIndex > 0)
	{
		row->Remove();
	}
	else
	{
		note->Remove();
	}

	if (onChar)
	{
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		pagingBuffer->Remove();
	}
}

Command* WriteAtEndCommand::Clone() {
	return new WriteAtEndCommand(*this);
}