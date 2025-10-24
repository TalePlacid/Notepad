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

	this->rowStartIndex = 0;
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

Glyph* PagingBuffer::Load() {
	//1. 줄의 처음으로 이동한다.
	Long currentOffset = ftell(this->file);
	FilePointerCalculator filePointerCaculator(this);
	Long offset = filePointerCaculator.First(currentOffset);
	fseek(this->file, offset, SEEK_SET);
	
	//2. 바이트수가 페이지 크기보다 작고, 줄수가 적재 줄수보다 작으면 반복한다.
	Long fileEndOffset = filePointerCaculator.FileEnd();
	Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;
	Long rowCount = 0;
	TCHAR(*contents) = new TCHAR[this->pageSize+1];
	Long i = 0;
	while (i < fileEndOffset && i < this->pageSize && rowCount < loadingRowCount)
	{
		//2.1. 내용을 읽는다.
		fread(contents + i, 1, 1, this->file);
		if (contents[i] == '\n')
		{
			rowCount++;
		}
		i++;
	}

	//3. 페이지 크기를 넘어섰다면,
	ByteChecker byteChecker;
	if (i >= this->pageSize && (byteChecker.IsLeadByte(contents[--i]) || contents[--i] == '\r'))
	{
		i--;
	}
	contents[i] = '\0';

	//4. 기존 위치로 이동한다.
	fseek(this->file, currentOffset, SEEK_SET);

	//5. 노트로 전환한다.
	NoteConverter noteConverter;
	Glyph* note = noteConverter.Convert(contents);

	if (contents != NULL)
	{
		delete[] contents;
	}

	return note;
#if 0
	//1. 적재할 줄 수를 구한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;

	//2. 시작위치를 구한다.
	FilePointerCalculator filePointerCalculator(this);
	PositionCalculator positionCalculator(this);
	Long currentOffset = ftell(this->file);

	Long aboveLineCount = loadingRowCount / 2 - 1;
	Long lineCount = 0;
	Long previousOffset = -1;
	Long offset = currentOffset;
	while (lineCount < aboveLineCount && offset != previousOffset)
	{
		previousOffset = offset;
		offset = filePointerCalculator.PreviousRow(offset);
		if (previousOffset != offset)
		{
			lineCount++;
		}
	}

	offset = filePointerCalculator.First(offset);

	this->startOffset = offset;
	this->start = Position(0, 0);

	//3. 현재 좌표를 구한다.
	Long i = 0;
	while (i < lineCount)
	{
		offset = filePointerCalculator.NextRow(offset);
		i++;
	}
	this->current = Position(this->start.GetRow() + lineCount, 0);

	while (offset < currentOffset)
	{
		previousOffset = offset;
		offset = filePointerCalculator.Next(offset);
		this->current = positionCalculator.Next(this->current, previousOffset);
	}

	//4. 끝 위치를 구한다.
	this->end = this->current;
	previousOffset = -1;
	Long belowLineCount = loadingRowCount - (lineCount + 1);
	lineCount = 0;
	while (lineCount < belowLineCount && previousOffset != offset && (offset - this->startOffset) <= this->pageSize)
	{
		previousOffset = offset;
		offset = filePointerCalculator.NextRow(offset);
		this->end = positionCalculator.NextRow(this->end, previousOffset);
		lineCount++;
	}

	//페이지 크기를 넘고, 마지막줄이면,
	if ((offset - this->startOffset) > this->pageSize && previousOffset == offset)
	{
		//현재 줄에서 가장 가까운 글자까지만 적재한다.
		previousOffset = offset;
		offset = filePointerCalculator.First(offset);
		this->end = positionCalculator.First(this->end);

		Long pageEndOffset = this->startOffset + this->pageSize;
		while (offset < pageEndOffset)
		{
			previousOffset = offset;
			offset = filePointerCalculator.Next(offset);
			this->end = positionCalculator.Next(this->end, previousOffset);
		}

		if (offset > pageEndOffset)
		{
			previousOffset = offset;
			offset = filePointerCalculator.Previous(offset);
			this->end = positionCalculator.Previous(this->end);
		}
	}
	else
	{
		// 페이지 크기를 넘으면, 이전줄로 이동한다.
		if ((offset - this->startOffset) > this->pageSize)
		{
			previousOffset = offset;
			offset = filePointerCalculator.PreviousRow(offset);
			this->end = positionCalculator.PreviousRow(this->end, previousOffset);
		}

		// 마지막 위치로 이동한다.
		previousOffset = offset;
		offset = filePointerCalculator.Last(offset);
		this->end = positionCalculator.Last(this->end, previousOffset);
	}
	this->endOffset = offset;

	//5. 페이지를 읽는다.
	fseek(this->file, this->startOffset, SEEK_SET);
	TCHAR(*contents) = new TCHAR[this->pageSize];
	Long contentsSize = this->endOffset - this->startOffset;
	Long count = fread(contents, 1, contentsSize, this->file);
	contents[count] = '\0';

	//6. 노트로 변환한다.
	NoteConverter noteConverter;
	Glyph *note = noteConverter.Convert(contents);

	if (contents != NULL)
	{
		delete[] contents;
	}

	return note;
#endif
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
		TCHAR(*contents) = new TCHAR[fileEndOffset + characterLength + 1];

		fseek(this->file, 0, SEEK_SET);
		fread(contents, 1, currentOffset, this->file);

		memcpy(contents + currentOffset, character, characterLength);
		
		Long backwardStart = currentOffset + characterLength;
		fseek(this->file, currentOffset, SEEK_SET);
		fread(contents + backwardStart, 1, fileEndOffset - currentOffset, this->file);
		contents[fileEndOffset + characterLength] = '\0';

		fwrite(contents, 1, fileEndOffset + characterLength, addedFile);

		if (this->selectionBeginOffset > currentOffset)
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
		this->endOffset += characterLength;
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

Long PagingBuffer::Add(CString str) {
	FILE* addedFile = fopen("AddedFile.tmp", "w+b");

	if (addedFile != NULL)
	{
		Long currentOffset = ftell(this->file);
	
		FilePointerCalculator filePointerCalculator(this);
		Long fileEndOffset = filePointerCalculator.FileEnd();

		TCHAR(*contents) = new TCHAR[fileEndOffset + str.GetLength() + 1];

		fseek(this->file, 0, SEEK_SET);
		fread(contents, 1, currentOffset, this->file);

		memcpy(contents + currentOffset, str.GetBuffer(), str.GetLength());

		Long backwardStart = currentOffset + str.GetLength();
		fseek(this->file, currentOffset, SEEK_SET);
		fread(contents + backwardStart, 1, fileEndOffset - currentOffset, this->file);
		contents[fileEndOffset + str.GetLength()] = '\0';

		fwrite(contents, 1, fileEndOffset + str.GetLength(), addedFile);

		if (this->selectionBeginOffset > currentOffset)
		{
			this->selectionBeginOffset += str.GetLength();
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
		fseek(this->file, currentOffset + str.GetLength(), SEEK_SET);

		TCHAR character[2];
		ByteChecker byteChecker;
		Long i = 0;
		while (i < str.GetLength())
		{
			character[0] = str.GetAt(i);
			if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
			{
				character[1] = str.GetAt(++i);
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
			i++;
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
	}

	return ret;
}

Long PagingBuffer::Replace(Long offset, CString str) {
	FILE* replacedFile = fopen("replacedFile.tmp", "w+b");

	if (replacedFile != NULL)
	{
		FilePointerCalculator filePointerCalculator(this);
		Long fileEndOffset = filePointerCalculator.FileEnd();

		TCHAR(*contents) = new TCHAR[fileEndOffset + 1];
		
		fseek(this->file, 0, SEEK_SET);
		fread(contents, 1, offset, this->file);
		
		memcpy(contents + offset, str.GetBuffer(), str.GetLength());
		fseek(this->file, offset + str.GetLength(), SEEK_SET);

		fread(contents + (offset+str.GetLength()), 1, fileEndOffset - (offset + str.GetLength()), this->file);
		contents[fileEndOffset] = '\0';

		fwrite(contents, 1, fileEndOffset, replacedFile);
		
		fclose(replacedFile);
		fclose(this->file);

		remove("Note.tmp");
		rename("replacedFile.tmp", "Note.tmp");
		this->file = fopen("Note.tmp", "r+b");

		fseek(this->file, offset + str.GetLength(), SEEK_SET);
	}

	return ftell(this->file);
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

	return count + 1;
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

bool PagingBuffer::IsOnPage(Long offset) {
	bool ret = false;
	if (offset >= this->startOffset && offset <= this->endOffset)
	{
		ret = true;
	}

	return ret;
}

Long PagingBuffer::MoveOffset(Long offset) {
	Long currentOffset = ftell(this->file);
	Long oldCurrentOffset = -1;

	FilePointerCalculator filePointerCalculator(this);
	PositionCalculator positionCalculator(this);
	if (currentOffset > offset)
	{
		oldCurrentOffset = currentOffset;
		currentOffset = filePointerCalculator.First(currentOffset);
		fseek(this->file, currentOffset, SEEK_SET);
		this->current = positionCalculator.First(this->current);

		while (currentOffset > offset)
		{
			oldCurrentOffset = currentOffset;
			currentOffset = filePointerCalculator.PreviousRow(currentOffset);
			fseek(this->file, currentOffset, SEEK_SET);
			this->current = positionCalculator.PreviousRow(this->current, oldCurrentOffset);
		}

		while (currentOffset < offset)
		{
			oldCurrentOffset = currentOffset;
			currentOffset = filePointerCalculator.Next(currentOffset);
			fseek(this->file, currentOffset, SEEK_SET);
			this->current = positionCalculator.Next(this->current, oldCurrentOffset);
		}
	}
	else if (currentOffset < offset)
	{
		while (currentOffset < offset && oldCurrentOffset != currentOffset)
		{
			oldCurrentOffset = currentOffset;
			currentOffset = filePointerCalculator.NextRow(currentOffset);
			fseek(this->file, currentOffset, SEEK_SET);
			this->current = positionCalculator.NextRow(this->current, oldCurrentOffset);
		}

		if (currentOffset > offset)
		{
			oldCurrentOffset = currentOffset;
			currentOffset = filePointerCalculator.PreviousRow(currentOffset);
			fseek(this->file, currentOffset, SEEK_SET);
			this->current = positionCalculator.PreviousRow(this->current, oldCurrentOffset);
		}

		while (currentOffset < offset)
		{
			oldCurrentOffset = currentOffset;
			currentOffset = filePointerCalculator.Next(currentOffset);
			fseek(this->file, currentOffset, SEEK_SET);
			this->current = positionCalculator.Next(this->current, oldCurrentOffset);
		}
	}

	return ftell(this->file);
}

Long PagingBuffer::CacheRowStartIndex(Long difference) {
	this->rowStartIndex += difference;

	return this->rowStartIndex;
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

CString PagingBuffer::GetFullText() {
	Long currentOffset = ftell(this->file);

	fseek(this->file, 0, SEEK_END);
	Long fileEndOffset = ftell(this->file);

	TCHAR(*contents) = new TCHAR[fileEndOffset+1];

	fseek(this->file, 0, SEEK_SET);
	fread(contents, 1, fileEndOffset, this->file);
	contents[fileEndOffset] = '\0';

	CString fullText(contents);

	if (contents != NULL)
	{
		delete[] contents;
	}

	fseek(this->file, currentOffset, SEEK_SET);

	return fullText;
}

Long PagingBuffer::GetCurrentOffset() const {
	return ftell(this->file);
}

Long PagingBuffer::GetFileEndOffset() const {
	FilePointerCalculator filePointerCalculator(const_cast<PagingBuffer*>(this));
	return filePointerCalculator.FileEnd();
}