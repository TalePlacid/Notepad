#include <afxwin.h>
#include <WinBase.h>
#include <io.h>
#include <fcntl.h>
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "NoteConverter.h"
#include "SizeCalculator.h"
#include "ByteChecker.h"

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
	((NotepadForm*)(this->parent))->note = noteConverter.Convert(contents, false);
	this->isDirty = false;

	//8. 노트에서 현재 위치로 이동한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(this->current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(this->current.GetColumn());

	//9. 선택시작위치가 표시되어 있으면,
	if (this->selectionBeginOffset >= 0)
	{
		Long j;
		Position selectionBegin;
		//9.1. 선택시작위치가 현재위치보다 위이면,
		if (this->selectionBeginOffset < currentOffset)
		{
			selectionBegin = this->start;
			//9.1.1. 선택시작위치가 시작위치보다 아래이면,
			if (this->selectionBeginOffset > this->startOffset)
			{
				//9.1.1.1. 선택시작위치의 좌표를 구한다.
				Long i = this->startOffset;
				fseek(this->file, i, SEEK_SET);
				while (i < this->selectionBeginOffset)
				{
					fread(character, 1, 1, this->file);

					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
						i++;
					}

					if (character[0] != '\r')
					{
						selectionBegin = selectionBegin.Right();
					}
					else
					{
						selectionBegin = selectionBegin.Down();
						selectionBegin = selectionBegin.Move(selectionBegin.GetRow(), 0);
					}

					i++;
				}
			}
			
			//9.1.2. 선택시작좌표부터 현재좌표까지 선택한다.
			row = note->GetAt(selectionBegin.GetRow());
			i = selectionBegin.GetColumn();
			while (i < row->GetLength())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}

			i = selectionBegin.GetRow() + 1;
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
		else if (this->selectionBeginOffset > currentOffset) //9.2. 선택시작위치가 현재위치보다 아래이면,
		{
			selectionBegin = this->end;
			//9.2.1. 선택시작위치가 끝위치보다 위이면,
			if (this->selectionBeginOffset < this->endOffset)
			{
				//9.2.1.1. 선택시작위치의 좌표를 구한다.
				selectionBegin = this->current;
				i = currentOffset;
				fseek(this->file, i, SEEK_SET);
				while (i < selectionBeginOffset)
				{
					fread(character, 1, 1, this->file);
					
					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
						i++;
					}
		
					if (character[0] != '\r')
					{
						selectionBegin = selectionBegin.Right();
					}
					else
					{
						selectionBegin = selectionBegin.Down();
						selectionBegin = selectionBegin.Move(selectionBegin.GetRow(), 0);
					}

					i++;
				}
			}

			//9.2.2. 현재좌표부터 선택시작좌표까지 선택한다.
			row = note->GetAt(this->current.GetRow());
			i = this->current.GetColumn();
			while (i < row->GetLength())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}

			i = this->current.GetRow() + 1;
			while (i < selectionBegin.GetRow())
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

			row = note->GetAt(selectionBegin.GetRow());
			i = 0;
			while (i < selectionBegin.GetColumn())
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

Long PagingBuffer::Add(char(*character)) {
	Long currentOffset = ftell(this->file);

	//1. 새 임시파일을 만든다.
	FILE* addedFile = fopen("AddedFile.tmp", "w+b");

	if (addedFile != NULL)
	{
		//2. 글자 크기를 구한다.
		Long characterLength = 1;
		ByteChecker byteChecker;
		if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
		{
			characterLength = 2;
		}

		//3. 임시파일에 문자를 추가한 전체파일을 옮겨적는다.
		fseek(this->file, 0, SEEK_END);
		Long fileEndOffset = ftell(this->file);
		TCHAR(*contents) = new TCHAR[fileEndOffset + characterLength];

		fseek(this->file, 0, SEEK_SET);
		fread(contents, 1, currentOffset, this->file);

		memcpy(contents + currentOffset, character, characterLength);
		
		Long backwardStart = currentOffset + characterLength;
		fseek(this->file, currentOffset, SEEK_SET);
		fread(contents + backwardStart, 1, fileEndOffset - currentOffset, this->file);

		fwrite(contents, 1, fileEndOffset + characterLength - 1, addedFile);

		if (contents != NULL)
		{
			delete[] contents;
		}

		//4. 임시파일과 기존파일을 맞바꾼다.
		fclose(addedFile);
		fclose(this->file);
		remove("Note.tmp");
		rename("AddedFile.tmp", "Note.tmp");
		this->file = fopen("Note.tmp", "r+b");

		//5. 페이징 버퍼에서 노트와의 맵핑정보를 갱신한다.
		fseek(this->file, currentOffset + characterLength, SEEK_SET);
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

	return ftell(this->file);
}

Long PagingBuffer::Remove() {
	Long ret = 0;
	Long currentOffset = ftell(this->file);

	//1. 새 임시파일을 만든다.
	FILE* removedFile = fopen("RemovedFile.tmp", "w+b");
	if (removedFile != NULL)
	{
		//2. 지울 문자의 길이를 구한다.
		fseek(this->file, currentOffset - 1, SEEK_SET);
		TCHAR character;
		fread(&character, 1, 1, this->file);

		Long characterLength = 1;
		ByteChecker byteChecker;
		if (!byteChecker.IsASCII(character) || character == '\n')
		{
			characterLength = 2;
		}

		//3. 지울 문자를 제외하고 내용을 읽는다.
		fseek(this->file, 0, SEEK_END);
		Long fileEnd = ftell(this->file);
		TCHAR(*contents) = new TCHAR[fileEnd];

		Long withoutCharacterOffset = currentOffset - characterLength;
		fseek(this->file, 0, SEEK_SET);
		fread(contents, 1, withoutCharacterOffset, this->file);
		fseek(this->file, currentOffset, SEEK_SET);
		fread(contents + withoutCharacterOffset, 1, fileEnd - currentOffset, this->file);
		
		//4. 새 파일에 내용을 쓴다.
		fwrite(contents, 1, fileEnd - characterLength, removedFile);
		
		if (contents != NULL)
		{
			delete[] contents;
		}

		//5. 새 파일과 기존파일을 맞바꾼다.
		fclose(removedFile);
		fclose(this->file);
		remove("Note.tmp");
		rename("RemovedFile.tmp", "Note.tmp");
		this->file = fopen("Note.tmp", "r+b");

		//6. 페이징 버퍼에서 노트와의 맵핑정보를 갱신한다.
		if (character != '\n')
		{
			this->current = this->current.Left();
		}
		else
		{
			//이전 줄 첫번째 칸으로 이동.
			TCHAR letter[2];
			Long i = currentOffset - 3;
			fseek(this->file, i, SEEK_SET);
			fread(letter, 1, 1, this->file);
			while (i >= 0 && letter[0] != '\n')
			{
				i--;
				fseek(this->file, i, SEEK_SET);
				fread(letter, 1, 1, this->file);
			}

			if (i >= 0)
			{
				i++;
				this->current = this->current.Up();
				this->current = this->current.Move(this->current.GetRow(), 0);
			}
			else
			{
				i = 0;
				this->current = this->current.Move(0, 0);
			}
			fseek(this->file, i, SEEK_SET);

			//현재 칸까지 센다.
			while (i < currentOffset - characterLength)
			{
				fread(letter, 1, 1, this->file);
				if (byteChecker.IsLeadByte(letter[0]))
				{
					fread(letter + 1, 1, 1, this->file);
					i++;
				}

				this->current = this->current.Right();
				i++;
			}
		}
		fseek(this->file, currentOffset - characterLength, SEEK_SET);
		ret = -1;
	}

	return ret;
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
	while (i >= 0 && character != '\n' && result == 0)
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

	if (currentOffset > 0)
	{
		fseek(this->file, currentOffset - 1, SEEK_SET);
		TCHAR character;
		fread(&character, 1, 1, this->file);

		ByteChecker byteChecker;
		if (!byteChecker.IsASCII(character))
		{
			fseek(this->file, currentOffset - 2, SEEK_SET);
			this->current = this->current.Left();
		}
		else
		{
			if (character != '\n')
			{
				fseek(this->file, currentOffset - 1, SEEK_SET);
				this->current = this->current.Left();
			}
			else
			{
				fseek(this->file, currentOffset, SEEK_SET);
			}
		}
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
		flag = fread(character, 1, 1, this->file);
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
	//1. 현재 위치를 읽는다.
	Long currentOffset = ftell(this->file);

	//2. 이동할 줄이 현재 줄보다 이전이면,
	TCHAR character[2];
	Long need;
	Long i;
	Long count = 0;
	if (index < this->current.GetRow())
	{
		//2.1. 이동할 줄을 찾는다.
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

		//2.2. 찾았으면,
		if (count >= need)
		{  
			//2.2.1. 이동한다.
			currentOffset = i + 1;
		}
		else //2.3. 찾지 못했으면,
		{
			//2.3.1. 처음 위치로 이동한다.
			currentOffset = 0;
			index = 0;
		}

		fseek(this->file, currentOffset, SEEK_SET);
		this->current = this->current.Move(index, 0);
	}
	else if (index > this->current.GetRow()) //3. 이동할 줄이 현재 줄보다 다음이면,
	{
		//3.1. 이동할 줄을 찾는다.
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

		//3.2. 찾았으면,
		if (count >= need && !feof(this->file))
		{
			//3.2.1. 이동한다.
			fseek(this->file, -1, SEEK_CUR);
			this->current = this->current.Move(index, 0);
		}
		else //3.3. 찾지 못했으면,
		{
			//3.3.1. 마지막 줄의 시작으로 이동한다.
			i = ftell(this->file) - 1;
			fseek(this->file, i, SEEK_SET);
			fread(character, 1, 1, this->file);
			while (i > currentOffset && character[0] != '\n')
			{
				i--;
				fseek(this->file, i, SEEK_SET);
				fread(character, 1, 1, this->file);
			}
			this->current = this->current.Move(this->current.GetRow() + count, 0);

			//3.3.2. 파일의 마지막으로 이동한다.
			i = ftell(this->file);

			fseek(this->file, 0, SEEK_END);
			Long fileEnd = ftell(this->file);

			fseek(this->file, i, SEEK_SET);
			flag = fread(character, 1, 1, this->file);
			while (i < fileEnd && flag > 0 && !feof(this->file))
			{
				if (character[0] & 0x80)
				{
					flag = fread(character + 1, 1, 1, this->file);
					i++;
				}

				this->current = this->current.Right();
				i++;
			}
		}
	}

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

CString PagingBuffer::MakeSelectedString() {
	CString selectedString("");
	if (this->selectionBeginOffset >= 0)
	{
		Long currentOffset = ftell(this->file);

		Long forwardOffset;
		Long backwardOffset;
		if (currentOffset < this->selectionBeginOffset)
		{
			forwardOffset = currentOffset;
			backwardOffset = this->selectionBeginOffset;
		}
		else if (currentOffset > this->selectionBeginOffset)
		{
			forwardOffset = this->selectionBeginOffset;
			backwardOffset = currentOffset;
		}

		Long length = backwardOffset - forwardOffset;
		TCHAR(*contents) = new TCHAR[length + 1];

		fseek(this->file, forwardOffset, SEEK_SET);
		fread(contents, 1, length, this->file);
		contents[length] = '\0';

		selectedString = CString(contents);

		if (contents != NULL)
		{
			delete[] contents;
		}
		fseek(this->file, currentOffset, SEEK_SET);
	}

	return selectedString;
}

bool PagingBuffer::MarkAsDirty() {
	this->isDirty = true;

	return this->isDirty;
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