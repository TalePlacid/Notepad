#include <io.h>
#include <fcntl.h>
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "NoteConverter.h"
#include "SizeCalculator.h"
#include "ByteChecker.h"
#include "FilePointerCalculator.h"
#include "PositionCalculator.h"

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

		if (this->selectionBeginOffset >= currentOffset)
		{
			this->selectionBeginOffset += characterLength;
		}

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

	//1. 새 임시파일을 만든다.
	FILE* removedFile = fopen("RemovedFile.tmp", "w+b");
	if (removedFile != NULL)
	{
		//2. 지울 문자의 길이를 구한다.
		Long currentOffset = ftell(this->file);
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

		if (this->selectionBeginOffset >= currentOffset)
		{
			this->selectionBeginOffset -= characterLength;
		}

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

Long PagingBuffer::Remove(Long toOffset) {
	Long ret = 0;

	FILE* removedFile = fopen("RemovedFile.tmp", "w+b");
	if (removedFile != NULL) {
		Long currentOffset = ftell(this->file);

		FilePointerCalculator filePointerCalculator(this);
		Long fileEndOffset = filePointerCalculator.FileEnd();

		TCHAR(*contents) = new TCHAR[fileEndOffset];

		Long forwardOffset = 0;
		Long backwardOffset = 0;
		if (currentOffset < toOffset)
		{
			forwardOffset = currentOffset;
			backwardOffset = toOffset;
		}
		else if (currentOffset > toOffset)
		{
			forwardOffset = toOffset;
			backwardOffset = currentOffset;
		}

		Long count = 0;
		fseek(this->file, 0, SEEK_SET);
		count += fread(contents, 1, forwardOffset, this->file);
		fseek(this->file, backwardOffset, SEEK_SET);
		count += fread(contents + forwardOffset, 1, fileEndOffset - backwardOffset, this->file);
		contents[count] = '\0';

		fwrite(contents, 1, count, removedFile);

		if (contents != NULL)
		{
			delete[] contents;
		}

		fclose(removedFile);
		fclose(this->file);
		remove("Note.tmp");
		rename("removedFile.tmp", "Note.tmp");
		this->file = fopen("Note.tmp", "r+b");

		fseek(this->file, forwardOffset, SEEK_SET);
		
		Long forwardRemovedOffset;
		if (this->startOffset <= forwardOffset)
		{
			forwardRemovedOffset = forwardOffset;
		}
		else
		{
			forwardRemovedOffset = this->startOffset;
		}

		Long backwardRemovedOffset;
		if (this->endOffset <= backwardOffset)
		{
			backwardRemovedOffset = this->endOffset;
		}
		else
		{
			backwardRemovedOffset = backwardOffset;
		}

		if (this->startOffset > forwardOffset)
		{
			this->startOffset = forwardOffset;
		}

		this->endOffset -= backwardRemovedOffset - forwardRemovedOffset;

		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		this->current = Position(rowIndex, columnIndex);
		Long endIndex = note->GetLength() - 1;
		row = note->GetAt(endIndex);
		Long endColumn = row->GetLength();
		this->end = Position(endIndex, endColumn);

		ret = -1;
#if 0
		if (currentOffset < toOffset)
		{
			count += fread(contents, 1, currentOffset, this->file);
			fseek(this->file, toOffset, SEEK_SET);
			count += fread(contents + currentOffset, 1, fileEndOffset - toOffset, this->file);
		}
		else
		{
			count += fread(contents, 1, toOffset, this->file);
			fseek(this->file, currentOffset, SEEK_SET);
			count += fread(contents + toOffset, 1, fileEndOffset - currentOffset, this->file);
		}
		contents[count] = '\0';

		fwrite(contents, 1, count, removedFile);

		if (contents != NULL)
		{
			delete[] contents;
		}

		fclose(removedFile);
		fclose(this->file);
		remove("Note.tmp");
		rename("removedFile.tmp", "Note.tmp");
		this->file = fopen("Note.tmp", "r+b");

		if (currentOffset < toOffset)
		{
			fseek(this->file, currentOffset, SEEK_SET);
		}
		else
		{
			fseek(this->file, toOffset, SEEK_SET);
		}

		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		PositionCalculator positionCalculator(this);

		ret = -1;

#endif
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

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.First(currentOffset), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.First(this->current);

	return this->current;
}

Position& PagingBuffer::Previous() {
	Long currentOffset = ftell(this->file);
	
	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.Previous(currentOffset), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.Previous(this->current);

	return this->current;
}

Position& PagingBuffer::Next() {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.Next(currentOffset), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.Next(this->current, currentOffset);

	return this->current;
}

Position& PagingBuffer::Last() {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.Last(currentOffset), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.Last(this->current, currentOffset);

	return this->current;
}

Position& PagingBuffer::Move(Long index) {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.Move(currentOffset, index), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.Move(this->current, index);

	return this->current;
}

Position& PagingBuffer::FirstRow() {
	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.FirstRow(), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.FirstRow();

	return this->current;
}

Position& PagingBuffer::PreviousRow() {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.PreviousRow(currentOffset), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.PreviousRow(this->current, currentOffset);

	return this->current;
}

Position& PagingBuffer::NextRow() {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.NextRow(currentOffset), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.NextRow(this->current, currentOffset);

	return this->current;
}

Position& PagingBuffer::LastRow() {
	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.LastRow(), SEEK_SET);

	PositionCalculator positionCalculator(this);
	this->current = positionCalculator.LastRow();

	return this->current;
}

Position& PagingBuffer::MoveRow(Long index) {
	//1. 현재 위치를 읽는다.
	Long currentOffset = ftell(this->file);

	//2. 이동할 줄이 현재 줄보다 아래이면,
	FilePointerCalculator filePointerCalculator(this);
	PositionCalculator positionCalculator(this);
	Long difference = index - this->current.GetRow();
	if (difference > 0)
	{
		//2.1. 차이만큼 아랫줄로 이동한다.
		fseek(this->file, filePointerCalculator.MoveDownRows(currentOffset, difference), SEEK_SET);
		this->current = positionCalculator.MoveDownRows(this->current, currentOffset, difference);
	}
	else if (difference < 0) //3. 이동할 줄이 현재 줄보다 위이면,
	{
		//3.1. 차이만큼 윗줄로 이동한다.
		fseek(this->file, filePointerCalculator.MoveUpRows(currentOffset, -difference), SEEK_SET);
		this->current = positionCalculator.MoveUpRows(this->current, currentOffset, -difference);
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

Long PagingBuffer::GetFileEndOffset() const {
	FilePointerCalculator filePointerCalculator(const_cast<PagingBuffer*>(this));
	return filePointerCalculator.FileEnd();
}