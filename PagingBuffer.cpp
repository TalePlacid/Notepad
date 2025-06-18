#include <afxwin.h>
#include <WinBase.h>
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "NoteConverter.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

PagingBuffer::PagingBuffer(CWnd* parent, Long pageSize) {
	this->parent = parent;
	this->pageSize = pageSize;

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	TCHAR directory[256];
	GetCurrentDirectory(256, directory);
	CString fileName = CString(directory) + CString("\\Note.tmp");

	this->file = fopen((LPCTSTR)fileName, "w+");
	if (this->file != NULL)
	{
		CString contents(notepadForm->Load(notepadForm->GetPath()));
		fwrite((LPCTSTR)contents, 1, contents.GetLength(), this->file);
		fseek(this->file, 0, SEEK_SET);
	}

	this->startOffset = 0;
	this->endOffset = 0;
	this->isDirty = false;
}

PagingBuffer::~PagingBuffer() {
	if (this->file != NULL)
	{
		fclose(this->file);
		TCHAR directory[256];
		GetCurrentDirectory(256, directory);
		CString fileName = CString(directory) + CString("\\Note.tmp");
		remove((LPCTSTR)fileName);
	}
}

void PagingBuffer::Load() {
	Long current = ftell(this->file);
	this->startOffset = current - (this->pageSize / 2 - 1);
	if (this->startOffset < 0)
	{
		this->startOffset = 0;
	}

	fseek(this->file, 0, SEEK_END);
	Long fileSize = ftell(this->file);

	this->endOffset = this->startOffset + this->pageSize - 1;
	if (this->endOffset > fileSize)
	{
		this->endOffset = fileSize;
	}

	TCHAR(*contents) = new TCHAR[this->pageSize + 1];
	fseek(this->file, this->startOffset, SEEK_SET);
	Long count = fread(contents, 1, this->pageSize, this->file);
	contents[count] = '\0';

	NoteConverter noteConverter;
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}
	
	((NotepadForm*)(this->parent))->note = noteConverter.ConvertToNote(contents);

	fseek(this->file, this->startOffset, SEEK_SET);
	this->current = this->current.Move(0, 0);
	TCHAR character[2];
	Long i = this->startOffset;
	size_t flag = fread(character, 1, 1, this->file);
	while (i < current && flag > 0 && !feof(this->file))
	{
		if (character[0] != '\n')
		{
			if (character[0] == '\r' || character[0] & 0x80)
			{
				i++;    
				flag = fread(character + 1, 1, 1, this->file);
			}

			if (character[0] != '\r')
			{
				this->current = this->current.Right();
			}
			else
			{
				this->current = this->current.Down();
				this->current = this->current.Move(this->current.GetRow(), 0);
			}
		}
		i++;
		flag = fread(&character, 1, 1, this->file);
	}
	fseek(this->file, current, SEEK_SET);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(this->current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(this->current.GetColumn());

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;
	Long rowCount = clientAreaHeight / ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();

	Long viewStartRow = this->current.GetRow() - (rowCount / 2 - 1);
	if (viewStartRow < 0)
	{
		viewStartRow = 0;
	}
	this->start = this->start.Move(viewStartRow, 0);

	Long viewEndRow = viewStartRow + rowCount - 1;
	if (viewEndRow > note->GetLength() - 1)
	{
		viewEndRow = note->GetLength() - 1;
	}

	row = note->GetAt(viewEndRow);
	Long viewEndColumn = row->GetLength() - 1;
	this->end = this->end.Move(viewEndRow, viewEndColumn);

	if (contents != NULL)
	{
		delete[] contents;
	}
}