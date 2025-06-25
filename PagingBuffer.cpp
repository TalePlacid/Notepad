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
	//1. 페이지 크기의 절반만큼 앞으로 이동한다.
	Long current = ftell(this->file);
	
	TCHAR character[2];
	Long offset = current - (this->pageSize / 2 - 1);
	if (offset >= 0)
	{
		int result = fseek(this->file, offset, SEEK_SET);
		fread(character, 1, 1, this->file);
		while (character[0] != '\n' && result == 0)
		{
			result = fseek(this->file, -2, SEEK_CUR);
			offset -= 2;
			fread(character, 1, 1, this->file);
		}

		if (character[0] == '\n')
		{
			this->startOffset = offset;
		}
		else
		{
			this->startOffset = 0;
		}
	}
	else
	{
		this->startOffset = 0;
	}

	fseek(this->file, 0, SEEK_END);
	Long lastOffset = ftell(this->file);

	//2. 파일에서 페이지 크기만큼 읽는다.
	TCHAR(*contents) = new TCHAR[this->pageSize + 1];
	fseek(this->file, this->startOffset, SEEK_SET);
	Long count = fread(contents, 1, this->pageSize, this->file);
	Long i = count - 1;
	while (i >= 0 && contents[i] != '\n')
	{
		i--;
	}

	if (i >= 0)
	{
		this->endOffset = this->startOffset + i;
	}
	else
	{
		this->endOffset = this->startOffset;
	}
	contents[i + 1] = '\0';

	NoteConverter noteConverter;
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}
	
	//3. 노트에 적재한다.
	((NotepadForm*)(this->parent))->note = noteConverter.ConvertToNote(contents);

	//4. 현재 위치를 구한다.
	fseek(this->file, this->startOffset, SEEK_SET);
	this->current = this->current.Move(0, 0);
	i = this->startOffset;
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

	//5. 노트에서 현재 위치로 이동한다.
	fseek(this->file, current, SEEK_SET);
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(this->current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(this->current.GetColumn());

	//6. 화면에 표시되는 줄 수를 구한다.
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowCount = clientAreaHeight / sizeCalculator->GetRowHeight();

	//7. 시작 위치를 구한다.
	Long startRow = this->current.GetRow() - (rowCount / 2 - 1);
	if (startRow < 0)
	{
		startRow = 0;
	}
	this->start = this->start.Move(startRow, 0);
	
	count = 0;
	Long countNeed = this->current.GetRow() - startRow;
	int result = fseek(this->file, -1, SEEK_CUR);
	flag = fread(character, 1, 1, this->file);
	while (count < countNeed && result == 0 && flag > 0)
	{
		if (character[0] == '\n')
		{
			count++;
		}

		result = fseek(this->file, -2, SEEK_CUR);
		flag = fread(character, 1, 1, this->file);
	}

	Long startOffset = ftell(this->file);
	if (result != 0)
	{
		fseek(this->file, 0, SEEK_SET);
		startOffset = 0;
	}

	//8. 끝 위치를 구한다.
	i = startOffset;
	count = 0;
	flag = fread(character, 1, 1, this->file);
	while (count < rowCount && i < this->endOffset && flag > 0 && !feof(this->file))
	{
		if (character[0] == '\r')
		{
			count++;
		}

		i++;
		flag = fread(character, 1, 1, this->file);
	}

	Long endRow = startRow + count - 1;
	row = note->GetAt(endRow);
	this->end = this->end.Move(endRow, row->GetLength());

	fseek(this->file, current, SEEK_SET);

#if 0
	//6. 수직 스크롤바의 정보를 읽는다.
	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	BOOL hasScrollBar = GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo);

	//7. 시작 위치를 구한다.
	Long startRowHeight = 0;
	if (hasScrollBar)
	{
		startRowHeight = scrollInfo.nPos;
	}

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long startRow = startRowHeight / sizeCalculator->GetRowHeight();
	this->start = this->start.Move(startRow, 0);

	//8. 끝 위치를 구한다.
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;
	Long rowCount = clientAreaHeight / ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();

	Long endRow = startRow + rowCount;
	row = note->GetAt(endRow);
	this->end = this->end.Move(endRow, row->GetLength() - 1);
#endif
	if (contents != NULL)
	{
		delete[] contents;
	}
}

Long PagingBuffer::CountRow() {
	Long current = ftell(this->file);

	Long count = 0;
	TCHAR character[2];
	fseek(this->file, 0, SEEK_SET);
	size_t flag = fread(character, 1, 1, this->file);
	while (flag > 0 && !feof(this->file))
	{
		if (character[0] == '\r' || character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
		}

		if (character[0] == '\r')
		{
			count++;
		}

		size_t flag = fread(character, 1, 1, this->file);
	}

	fseek(this->file, current, SEEK_SET);

	return count;
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
	while (i <= index && flag > 0 && !feof(this->file))
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
	Long current = ftell(this->file);

	TCHAR character[2];
	int result;
	Long need;
	Long count = 0;
	if (index < this->current.GetRow())
	{
		need = this->current.GetRow() - index + 1;
		result = fseek(this->file, -1, SEEK_CUR);
		while (count < need && result == 0)
		{
			fread(character, 1, 1, this->file);
			if (character[0] == '\n')
			{
				count++;
			}
			result = fseek(this->file, -2, SEEK_CUR);
		}

		if (result == 0)
		{
			fseek(this->file, 2, SEEK_CUR);
		}
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
	}

	this->current = this->current.Move(index, this->current.GetColumn());

	return this->current;
}

bool PagingBuffer::IsOnView() {
	bool ret = false;

	if (this->current >= this->start && this->current <= this->end)
	{
		ret = true;
	}

	return ret;
}