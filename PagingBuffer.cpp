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

	this->file = fopen((LPCTSTR)fileName, "w+b");
	if (this->file != NULL)
	{
		CString contents(notepadForm->Load(notepadForm->GetPath()));
		fwrite((LPCTSTR)contents, 1, contents.GetLength(), this->file);
		fseek(this->file, 0, SEEK_SET);
	}

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
	//1. 현재 포인터 위치를 구한다.
	Long currentOffset = ftell(this->file);

	//2. 적재할 줄 수를 구한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;
	Long rowCount = clientAreaHeight / sizeCalculator->GetRowHeight();
	Long loadingRowCount = rowCount * 4;

	//3. 시작위치를 구한다.
	TCHAR character[2];
	Long newLineNeeded = loadingRowCount / 2 + 1;
	Long newLineCount = 0;
	Long i = currentOffset - 1;
	while (newLineCount < newLineNeeded && i >= 0)
	{
		int ret = fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);

		if (character[0] == '\n')
		{
			newLineCount++;
		}

		i--;
	}

	this->startOffset = 0;
	if (newLineCount >= newLineNeeded)
	{
		this->startOffset = i;
	}
	this->start = this->start.Move(0, 0);

	//4. 현재 좌표를 구한다.
	newLineNeeded = loadingRowCount;
	newLineCount = 0;
	this->current = this->start;
	i = this->startOffset;
	fseek(this->file, i, SEEK_SET);
	while (i < currentOffset)
	{
		fread(character, 1, 1, this->file);

		if (character[0] == '\r' || character[0] & 0x80)
		{
			i++;
			fread(character + 1, 1, 1, this->file);
		}

		if (character[0] != '\r')
		{
			this->current = this->current.Right();
		}
		else
		{
			this->current = this->current.Down();
			this->current = this->current.Move(this->current.GetRow(), 0);
			newLineCount++;
		}

		i++;
	}

	//5. 끝 위치를 구한다.
	Position previous;
	fseek(this->file, currentOffset, SEEK_SET);
	this->end = this->current;
	size_t flag = fread(character, 1, 1, this->file);
	i = currentOffset + 1;
	Long pageLimit = this->startOffset + this->pageSize;
	while (newLineCount < newLineNeeded && flag > 0 && !feof(this->file) && i < pageLimit)
	{
		if (character[0] == '\r' || character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
			i++;
		}

		if (character[0] != '\r')
		{
			this->end = this->end.Right();
		}
		else
		{
			previous = this->end;
			this->end = this->end.Down();
			this->end = this->end.Move(this->end.GetRow(), 0);
			newLineCount++;
		}
		
		fread(character, 1, 1, this->file);
		i++;
	}

	if (newLineCount >= newLineNeeded)
	{
		this->endOffset = i - 3;
		this->end = previous;
	}
	else if (i > pageLimit)
	{
		Long j = pageLimit - 1;
		fseek(this->file, j, SEEK_SET);
		fread(character, 1, 1, this->file);
		while (j >= currentOffset && character[0] != '\r')
		{
			j--;
			fseek(this->file, j, SEEK_SET);
			fread(character, 1, 1, this->file);
		}

		this->endOffset = j;
		this->end = previous;
	}
	else if (feof(this->file))
	{
		fseek(this->file, 0, SEEK_END);
		this->endOffset = ftell(this->file);
	}

	//6. 페이지를 읽는다.
	fseek(this->file, this->startOffset, SEEK_SET);
	TCHAR(*contents) = new TCHAR[this->pageSize];
	Long contentsSize = this->endOffset - this->startOffset;
	Long count = fread(contents, 1, contentsSize, this->file);
	contents[count] = '\0';

	//7. 노트로 변환한다.
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}

	NoteConverter noteConverter;
	((NotepadForm*)(this->parent))->note = noteConverter.ConvertToNote(contents);

	//8. 노트에서 현재 위치로 이동한다.
	fseek(this->file, currentOffset, SEEK_SET);
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(this->current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(this->current.GetColumn());

	if (contents != NULL)
	{
		delete[] contents;
	}
}

Long PagingBuffer::CountRow(Long offset) {
	Long current = ftell(this->file);

	Long count = 0;
	TCHAR character[2];
	fseek(this->file, 0, SEEK_SET);
	Long i = 0;
	size_t flag = fread(character, 1, 1, this->file);
	while (i < offset && flag > 0 && !feof(this->file))
	{
		if (character[0] == '\r')
		{
			count++;
		}

		i++;
		size_t flag = fread(character, 1, 1, this->file);
	}

	fseek(this->file, current, SEEK_SET);

	return count;
}

bool PagingBuffer::IsAboveBottomLine() {
	bool ret = false;

	Long currentOffset = ftell(this->file);
	fseek(this->file, 0, SEEK_END);
	Long fileEnd = ftell(this->file);
	fseek(this->file, currentOffset, SEEK_SET);

	Long bottomLine = this->end.GetRow() / 10 * 8;
	if (this->current.GetRow() < bottomLine && this->endOffset >= fileEnd)
	{
		ret = true;
	}

	return ret;
}

bool PagingBuffer::IsBelowTopLine() {
	bool ret = false;

	Long topLine = this->end.GetRow() / 10 * 4;
	if (this->current.GetRow() > topLine && this->startOffset <= 0)
	{
		ret = true;
	}

	return ret;
}

Position& PagingBuffer::First() {
	Long currentOffset = ftell(this->file);

	TCHAR character;
	Long i = currentOffset - 1;
	int result = fseek(this->file, i, SEEK_SET);
	fread(&character, 1, 1, this->file);
	while (i >= 0 && character == '\n' && result == 0)
	{
		i--;
		result = fseek(this->file, i, SEEK_SET);
		fread(&character, 1, 1, this->file);
	}

	this->current = this->current.Move(this->current.GetRow(), 0);

	return this->current;
}

Position& PagingBuffer::Previous() {
	Long currentOffset = ftell(this->file);

	TCHAR character[2] = {'\0', '\0'};
	Long offset = currentOffset - 1;
	int result = fseek(this->file, offset, SEEK_SET);
	Long i = 1;
	while (i >= 0 && result == 0)
	{
		fread(character + i, 1, 1, this->file);
		i--;
		offset--;
		result = fseek(this->file, offset, SEEK_SET);
	}

	if (character[0] != '\0' || character[0] != '\r')
	{
		if (character[0] & 0x80)
		{
			fseek(this->file, currentOffset - 2, SEEK_SET);
		}
		else
		{
			fseek(this->file, currentOffset - 1, SEEK_SET);
		}
		this->current = this->current.Left();
	}
	else
	{
		fseek(this->file, currentOffset, SEEK_SET);
	}

	return this->current;
}

Position& PagingBuffer::Next() {
	Long currentOffset = ftell(this->file);

	TCHAR character[2];
	size_t flag = fread(character, 1, 1, this->file);
	if (flag > 0 && !feof(this->file))
	{
		if (character[0] == '\r' || character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
		}

		if (character[0] != '\r')
		{
			this->current = this->current.Right();
		}
		else
		{
			fseek(this->file, currentOffset, SEEK_SET);
		}
	}

	return this->current;
}

Position& PagingBuffer::Last() {
	Long currentOffset = ftell(this->file);
	Position current = this->current;

	TCHAR character[2];
	size_t flag = fread(character, 1, 1, this->file);
	while (character[0] != '\r' && flag > 0 && !feof(this->file))
	{
		if (character[0] == '\r' || character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
		}

		this->current.Right();
		flag = fread(character + 1, 1, 1, this->file);
	}

	if (character[0] == '\r')
	{
		fseek(this->file, -1, SEEK_CUR);
		this->current.Left();
	}
	else
	{
		fseek(this->file, currentOffset, SEEK_SET);
		this->current = current;
	}

	return this->current;
}

Position& PagingBuffer::Move(Long index) {
	Long current = ftell(this->file);

	TCHAR character[2];
	Long i = 0;
	size_t flag = fread(character, 1, 1, this->file);
	while (i < index && flag > 0 && !feof(this->file))
	{
		if (character[0] & 0x80 || character[0] == '\r')
		{
			flag = fread(character + 1, 1, 1, this->file);
		}

		i++;
	}

	if (!feof(this->file) && character[0] != '\r')
	{
		this->current = this->current.Move(this->current.GetRow(), index);
	}
	else
	{
		fseek(this->file, current, SEEK_SET);
	}

	return this->current;
}

Position& PagingBuffer::PreviousRow() {
	Long current = ftell(this->file);

	TCHAR character[2];
	Long count = 0;
	Long i = current - 1;
	while (i >= 0 && count < 2)
	{
		fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);

		if (character[0] == '\n')
		{
			count++;
		}

		i--;
	}

	if (count >= 2)
	{
		this->current = this->current.Up();
		this->current = this->current.Move(this->current.GetRow(), 0);
	}
	else
	{
		fseek(this->file, current, SEEK_SET);
		this->current = this->current.Move(0, 0);
	}

	return this->current;
}

Position& PagingBuffer::NextRow() {
	Long current = ftell(this->file);

	TCHAR character[2];
	size_t flag = fread(character, 1, 1, this->file);
	while (character[0] != '\n' && flag > 0 && !feof(this->file))
	{
		flag = fread(character, 1, 1, this->file);
	}

	if (!feof(this->file))
	{
		this->current = this->current.Move(this->current.GetRow() + 1, 0);
	}
	else
	{
		fseek(this->file, current, SEEK_SET);
	}

	return this->current;
}

Position& PagingBuffer::MoveRow(Long index) {
	Long currentOffset = ftell(this->file);

	TCHAR character[2];
	Long need;
	Long i;
	Long count = 0;
	if (index < this->current.GetRow())
	{
		need = this->current.GetRow() - index + 1;
		i = currentOffset - 1;
		while (count < need && i >= 0)
		{
			fseek(this->file, i, SEEK_SET);
			fread(character, 1, 1, this->file);

			if (character[0] == '\n')
			{
				count++;
			}
			
			i--;
		}

		if (count >= need)
		{  
			currentOffset = i + 1;
		}
		else
		{
			currentOffset = 0;
		}

		fseek(this->file, currentOffset, SEEK_SET);
	}
	else if (index > this->current.GetRow())
	{
		need = index - this->current.GetRow();
		size_t flag = fread(character, 1, 1, this->file);
		while (count < need && flag > 0 && !feof(this->file))
		{
			if (character[0] == '\n')
			{
				count++;
			}

			flag = fread(character, 1, 1, this->file);
		}

		if (count >= need && !feof(this->file))
		{
			fseek(this->file, -1, SEEK_CUR);
		}
	}

	this->current = this->current.Move(index, 0);

	return this->current;
}

Long PagingBuffer::GetFileEnd() const {
	Long current = ftell(this->file);

	fseek(this->file, 0, SEEK_END);
	Long fileEnd = ftell(this->file);

	fseek(this->file, current, SEEK_SET);

	return fileEnd;
}