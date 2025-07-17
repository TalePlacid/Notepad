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
	this->selectionBeginOffset = -1;
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
#if 0
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;
	Long rowCount = clientAreaHeight / sizeCalculator->GetRowHeight();
	Long loadingRowCount = rowCount * PAGE_MULTIPLE;
#endif
	Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;

	//3. 시작위치를 구한다.
	TCHAR character[2];
	Long newLineNeeded = loadingRowCount / 2 - 1;
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
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(this->current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(this->current.GetColumn());

	//9. 선택시작위치가 표시되어 있으면, 
	if (this->selectionBeginOffset >= 0)
	{
		Long j;

		//9.1 선택시작위치가 현재위치보다 위이면,
		if (this->selectionBeginOffset < currentOffset)
		{
			//9.1.1. 선택시작위치가 시작위치보다 아래이면,
			Position selectionStart(0, 0);
			if (this->selectionBeginOffset > this->startOffset)
			{
				//9.1.1.1. 선택시작좌표를 구한다.
				i = this->startOffset;
				while (i < this->selectionBeginOffset)
				{
					fseek(this->file, i, SEEK_SET);
					fread(character, 1, 1, this->file);

					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
					}

					if (character[0] != '\r')
					{
						selectionStart = selectionStart.Right();
					}
					else
					{
						selectionStart = selectionStart.Down();
						selectionStart = selectionStart.Move(selectionStart.GetRow(), 0);
					}

					i++;
				}
			}

			//9.1.2. 노트에서 선택시작좌표부터 현재좌표까지 선택한다.
			i = selectionStart.GetRow();
			if (i < this->current.GetRow())
			{
				row = note->GetAt(i);
				j = selectionStart.GetColumn();
				while (j < row->GetLength())
				{
					row->GetAt(j)->Select(TRUE);
					j++;
				}
			}

			i = selectionStart.GetRow() + 1;
			while (i < this->current.GetRow())
			{
				row = note->GetAt(i);
				j = 0;
				while (j < row->GetLength())
				{
					row->GetAt(j)->Select(TRUE);
					j++;
				}
				i++;
			}

			row = note->GetAt(this->current.GetRow());
			i = 0;
			while (i < this->current.GetColumn())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}
		}
		else if (this->selectionBeginOffset > currentOffset) //9.2 선택시작위치가 현재위치보다 아래이면,
		{
			//9.2.1.선택시작위치가 끝위치보다 위이면,
			Position selectionEnd = this->end;
			if (this->selectionBeginOffset < this->endOffset)
			{
				//9.2.1.1. 선택끝좌표를 구한다.
				i = this->selectionBeginOffset;
				while (i < this->selectionBeginOffset)
				{
					fseek(this->file, i, SEEK_SET);
					fread(character, 1, 1, this->file);

					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
					}

					if (character[0] != '\r')
					{
						selectionEnd = selectionEnd.Right();
					}
					else
					{
						selectionEnd = selectionEnd.Down();
						selectionEnd = selectionEnd.Move(selectionEnd.GetRow(), 0);
					}
					i++;
				}
			}

			//9.2.2. 노트에서 현재좌표부터 선택끝좌표까지 선택한다.
			row = note->GetAt(this->current.GetRow());
			i = this->current.GetColumn();
			if (i < row->GetLength())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}

			i = this->current.GetRow() + 1;
			while (i < selectionEnd.GetRow())
			{
				row = note->GetAt(i);
				j = 0;
				while (j < row->GetLength())
				{
					row->GetAt(j)->Select(TRUE);
					j++;
				}
				i++;
			}

			row = note->GetAt(selectionEnd.GetRow());
			i = 0;
			while (i < selectionEnd.GetColumn())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}
		}
	}

	//10. 현재 위치로 이동한다.
	fseek(this->file, currentOffset, SEEK_SET);

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
	if (this->current.GetRow() < bottomLine || this->endOffset >= fileEnd)
	{
		ret = true;
	}

	return ret;
}

bool PagingBuffer::IsBelowTopLine() {
	bool ret = false;

	Long topLine = this->end.GetRow() / 10 * 4;
	if (this->current.GetRow() > topLine || this->startOffset <= 0)
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
		if (character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
		}

		this->current = this->current.Right();
		flag = fread(character + 1, 1, 1, this->file);
	}

	if (character[0] == '\r')
	{
		fseek(this->file, -1, SEEK_CUR);
	}

	return this->current;
}

Position& PagingBuffer::Move(Long index) {
	Long current = ftell(this->file);

	TCHAR character[2];
	Long i = current - 1;
	int result = fseek(this->file, i, SEEK_SET);
	fread(character, 1, 1, this->file);
	while (i >= 0 && character[0] != '\n' && result == 0)
	{
		i--;
		result = fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);
	}

	if (result != 0)
	{
		fseek(this->file, 0, SEEK_SET);
	}

	i = 0;
	while (i < index)
	{
		fread(character, 1, 1, this->file);
		if (character[0] & 0x80)
		{
			fread(character + 1, 1, 1, this->file);
		}
		i++;
	}

	this->current = this->current.Move(this->current.GetRow(), index);

	return this->current;
}

Position& PagingBuffer::FirstRow() {
	fseek(this->file, 0, SEEK_SET);

	this->current = this->current.Move(0, 0);

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

	if (count > 0)
	{
		this->current = this->current.Up();
		this->current = this->current.Move(this->current.GetRow(), 0);
	}
	else
	{
		fseek(this->file, current, SEEK_SET);
	}

	return this->current;
}

Position& PagingBuffer::NextRow() {
	Long current = ftell(this->file);

	TCHAR character;
	size_t flag = fread(&character, 1, 1, this->file);
	while (character != '\n' && flag > 0 && !feof(this->file))
	{
		flag = fread(&character, 1, 1, this->file);
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

Position& PagingBuffer::LastRow() {
	//1. 파일의 끝으로 이동한다.
	fseek(this->file, 0, SEEK_END);
	Long fileEnd = ftell(this->file);

	//2. 마지막줄의 맨 앞으로 이동한다.
	TCHAR character[2];
	Long i = fileEnd - 1;
	int result = fseek(this->file, i, SEEK_SET);
	fread(character, 1, 1, this->file);
	while (i >= 0 && character[0] != '\n' && result == 0)
	{
		i--;
		result = fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);
	}

	if (result == 0) //3. 마지막 줄이 첫번째 줄이 아니면,
	{
		//3.1. 상단 적재되는 줄 수를 구한다.
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaHeight = rect.bottom - rect.top;
		Long rowCount = clientAreaHeight / ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();
		Long loadingRowCount = rowCount * PAGE_MULTIPLE;
		Long newLineNeeded = loadingRowCount / 2 - 1;

		//3.2. 실제 적재되는 줄 수를 구한다.
		Long newLineCount = 0;
		Long offset = i;
		fseek(this->file, offset, SEEK_SET);
		fread(character, 1, 1, this->file);
		while (offset >= 0 && newLineCount < newLineNeeded)
		{
			fseek(this->file, offset, SEEK_SET);
			fread(character, 1, 1, this->file);
			if (character[0] == '\n')
			{
				newLineCount++;
			}
			offset--;
		}

		//3.3. 마지막 줄의 첫번째 위치로 이동한다.
		fseek(this->file, i + 1, SEEK_SET);
		this->current = this->current.Move(newLineCount, 0);
	}
	else //4. 마지막 줄이 첫번째 줄이라면,
	{
		//4.1. 파일의 시작 위치로 이동한다.
		fseek(this->file, 0, SEEK_SET);
		this->current = this->current.Move(0, 0);
	}

	Long checkCurrentOffset = ftell(this->file);

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
			index = 0;
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
		else
		{
			index = this->current.GetRow() + count;
		}
	}

	this->current = this->current.Move(index, 0);

	return this->current;
}

Long PagingBuffer::MarkSelectionBegin() {
	this->selectionBeginOffset = ftell(this->file);

	return this->selectionBeginOffset;
}

Long PagingBuffer::UnmarkSelectionBegin() {
	this->selectionBeginOffset = -1;

	return this->selectionBeginOffset;
}

Long PagingBuffer::GetCurrentOffset() const {
	return ftell(this->file);
}

Long PagingBuffer::GetFileEnd() const {
	Long current = ftell(this->file);

	fseek(this->file, 0, SEEK_END);
	Long fileEnd = ftell(this->file);

	fseek(this->file, current, SEEK_SET);

	return fileEnd;
}