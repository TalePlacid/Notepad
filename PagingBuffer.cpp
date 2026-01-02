#include <io.h>
#include <fcntl.h>
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "NoteConverter.h"
#include "SizeCalculator.h"
#include "ByteChecker.h"
#include "FilePointerCalculator.h"

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

Glyph* PagingBuffer::LoadPrevious() {
	//1. 적재 크기만큼 위로 이동한다.
	TCHAR character;
	Long currentOffset = ftell(this->file);
	Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;
	Long rowCount = 0;
	Long i = 1;
	Long j = currentOffset - i;
	if (currentOffset > 0)
	{
		j -= 2;
	}
	while (j >= 0 && i < this->pageSize && rowCount < loadingRowCount)
	{
		fseek(this->file, j, SEEK_SET);
		fread(&character, 1, 1, this->file);
		if (character == '\n')
		{
			rowCount++;
		}
		i++;
		j--;
	}
	i--;
	j++;

	//2. 적재 크기를 넘어섰다면, 최대한의 문자까지를 범위로 한다.
	ByteChecker byteChecker;
	if (i >= this->pageSize)
	{
		fseek(this->file, j, SEEK_SET);
		fread(&character, 1, 1, this->file);
		if (byteChecker.IsTailByte(character) || character == '\n')
		{
			i--;
			j++;
		}
	}
	else if (rowCount >= loadingRowCount) //3. 적재줄수를 모두 채웠다면, 개행문자를 제외한다.
	{
		i--;
		j++;
	}

	//4. 적재 범위만큼 읽는다.
	TCHAR(*contents) = new TCHAR[this->pageSize];
	fseek(this->file, j, SEEK_SET);
	fread(contents, 1, i, this->file);
	contents[i] = '\0';

	//5. 노트로 변환한다.
	NoteConverter noteConverter;
	Glyph* note = noteConverter.Convert(contents);

	if (contents != NULL)
	{
		delete[] contents;
	}

	//6. 원래 위치로 돌아간다.
	fseek(this->file, currentOffset, SEEK_SET);

	return note;
}

Glyph* PagingBuffer::LoadNext() {
	//1. 바이트수가 페이지 크기보다 작고, 줄수가 적재 줄수보다 작으면 반복한다.
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCaculator(this);
	Long fileEndOffset = filePointerCaculator.FileEnd();
	Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;
	Long rowCount = 0;
	TCHAR(*contents) = new TCHAR[this->pageSize + 1];
	Long i = 0;
	while (ftell(this->file) < fileEndOffset && i < this->pageSize && rowCount < loadingRowCount)
	{
		//1.1. 내용을 읽는다.
		fread(contents + i, 1, 1, this->file);
		if (contents[i] == '\r')
		{
			rowCount++;
		}
		i++;
	}

	//2. 페이지 크기를 넘어섰다면, 최대한의 문자까지를 범위로 한다.
	ByteChecker byteChecker;
	if (i >= this->pageSize && (byteChecker.IsLeadByte(contents[i - 1]) || contents[i - 1] == '\r'))
	{
		i -= 2;
	}
	else if (rowCount >= loadingRowCount) //3. 적재줄 수를 모두 채웠다면, 줄바꿈 문자를 뺸 범위로 한다.
	{
		i -= 1;
	}
	contents[i] = '\0';

	//4. 기존 위치로 이동한다.
	fseek(this->file, currentOffset, SEEK_SET);

	//4. 노트로 전환한다.
	NoteConverter noteConverter;
	Glyph* note = noteConverter.Convert(contents);

	if (contents != NULL)
	{
		delete[] contents;
	}

	return note;
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

Long PagingBuffer::First() {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.First(currentOffset), SEEK_SET);

	return ftell(this->file);
}

Long PagingBuffer::Previous(Long count) {
	Long currentOffset = ftell(this->file);
	
	FilePointerCalculator filePointerCalculator(this);
	for (Long i = 1; i <= count; i++)
	{
		fseek(this->file, filePointerCalculator.Previous(currentOffset), SEEK_SET);
		currentOffset = ftell(this->file);
	}

	return ftell(this->file);
}

Long PagingBuffer::Next(Long count) {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	for (Long i = 1; i <= count; i++)
	{
		fseek(this->file, filePointerCalculator.Next(currentOffset), SEEK_SET);
		currentOffset = ftell(this->file);
	}

	return ftell(this->file);
}

Long PagingBuffer::Last() {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.Last(currentOffset), SEEK_SET);

	return ftell(this->file);
}

Long PagingBuffer::Move(Long index) {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.Move(currentOffset, index), SEEK_SET);

	return ftell(this->file);
}

Long PagingBuffer::FirstRow() {
	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.FirstRow(), SEEK_SET);

	return ftell(this->file);
}

Long PagingBuffer::PreviousRow(Long count) {
	Long currentOffset = ftell(this->file);

	FilePointerCalculator filePointerCalculator(this);
	Long i = 0;
	while (i < count)
	{
		fseek(this->file, filePointerCalculator.PreviousRow(currentOffset), SEEK_SET);
		currentOffset = ftell(this->file);
		i++;
	}

	return currentOffset;
}

Long PagingBuffer::NextRow(Long count) {
	FilePointerCalculator filePointerCalculator(this);
	Long currentOffset = ftell(this->file);

	Long i = 0;
	while (i < count)
	{
		fseek(this->file, filePointerCalculator.NextRow(currentOffset), SEEK_SET);
		currentOffset = ftell(this->file);
		i++;
	}

	return currentOffset;
}

Long PagingBuffer::LastRow() {
	FilePointerCalculator filePointerCalculator(this);
	fseek(this->file, filePointerCalculator.LastRow(), SEEK_SET);

	return ftell(this->file);
}

Long PagingBuffer::MarkSelectionBegin() {
	this->selectionBeginOffset = ftell(this->file);

	return this->selectionBeginOffset;
}

Long PagingBuffer::UnmarkSelectionBegin() {
	this->selectionBeginOffset = -1;

	return this->selectionBeginOffset;
}

bool PagingBuffer::BeginSelectionIfNeeded() {
	bool ret = false;

	if (this->selectionBeginOffset < 0)
	{
		this->selectionBeginOffset = ftell(this->file);
		ret = true;
	}

	return ret;
}

bool PagingBuffer::EndSelectionIfCollapsed() {
	bool ret = false;

	if (ftell(this->file) == this->selectionBeginOffset)
	{
		this->selectionBeginOffset = -1;
		ret = true;
	}

	return ret;
}

Long PagingBuffer::MoveOffset(Long offset) {
	fseek(this->file, offset, SEEK_SET);

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