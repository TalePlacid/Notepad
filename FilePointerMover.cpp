#include "FilePointerMover.h"
#include "PagingBuffer.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

FilePointerMover::FilePointerMover(PagingBuffer* pagingBuffer) {
	this->pagingBuffer = pagingBuffer;
}

FilePointerMover::~FilePointerMover() {

}

Long FilePointerMover::First() {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);
	TCHAR character;
	Long i = currentOffset - 1;
	int result = fseek(file, i, SEEK_SET);
	fread(&character, 1, 1, file);
	while (i >= 0 && character != '\n' && result == 0)
	{
		i--;
		result = fseek(file, i, SEEK_SET);
		fread(&character, 1, 1, file);
	}

	if (result != 0)
	{
		fseek(file, 0, SEEK_SET);
	}

	return ftell(file);
}

Long FilePointerMover::Previous() {
	FILE* file = this->pagingBuffer->GetFile();
	
	Long currentOffset = ftell(file);
	if (currentOffset > 0)
	{
		fseek(file, currentOffset - 1, SEEK_SET);
		TCHAR character;
		fread(&character, 1, 1, file);

		ByteChecker byteChecker;
		if (!byteChecker.IsASCII(character))
		{
			fseek(file, currentOffset - 2, SEEK_SET);
		}
		else if (character != '\n')
		{
			fseek(file, currentOffset - 1, SEEK_SET);
		}
	}

	return ftell(file);
}

Long FilePointerMover::Next() {
	FILE* file = this->pagingBuffer->GetFile();
	
	Long currentOffset = ftell(file);
	TCHAR character[2];
	size_t flag = fread(character, 1, 1, file);
	if (flag > 0 && !feof(file))
	{
		if (character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, file);
		}
		else if (character[0] == '\r')
		{
			fseek(file, currentOffset, SEEK_SET);
		}
	}

	return ftell(file);
}

Long FilePointerMover::Last() {
	FILE* file = this->pagingBuffer->GetFile();
	
	Long currentOffset = ftell(file);
	TCHAR character[2];
	size_t flag = fread(character, 1, 1, file);
	while (character[0] != '\r' && flag > 0 && !feof(file))
	{
		flag = fread(character, 1, 1, file);
	}

	if (character[0] == '\r')
	{
		fseek(file, -1, SEEK_CUR);
	}

	return ftell(file);
}

Long FilePointerMover::Move(Long index) {
	FILE* file = this->pagingBuffer->GetFile();
	
	Long currentOffset = ftell(file);
	TCHAR character[2];
	Long i = currentOffset - 1;
	int result = fseek(file, i, SEEK_SET);
	fread(character, 1, 1, file);
	while (i >= 0 && character[0] != '\n' && result == 0)
	{
		i--;
		result = fseek(file, i, SEEK_SET);
		fread(character, 1, 1, file);
	}

	if (result != 0)
	{
		fseek(file, 0, SEEK_SET);
	}

	i = 0;
	while (i < index)
	{
		fread(character, 1, 1, file);
		if (character[0] & 0x80)
		{
			fread(character + 1, 1, 1, file);
		}
		i++;
	}

	return ftell(file);
}

Long FilePointerMover::FirstRow() {
	FILE* file = this->pagingBuffer->GetFile();
	fseek(file, 0, SEEK_SET);

	return ftell(file);
}

Long FilePointerMover::PreviousRow() {
	FILE* file = this->pagingBuffer->GetFile();

	Long currentOffset = ftell(file);
	TCHAR character[2];
	Long count = 0;
	Long i = currentOffset - 1;
	while (i >= 0 && count < 2)
	{
		fseek(file, i, SEEK_SET);
		fread(character, 1, 1, file);

		if (character[0] == '\n')
		{
			count++;
		}

		i--;
	}

	if (count == 1)
	{
		fseek(file, 0, SEEK_SET);
	}
	else if (count == 0)
	{
		fseek(file, currentOffset, SEEK_SET);
	}

	return ftell(file);
}

Long FilePointerMover::NextRow() {
	FILE* file = this->pagingBuffer->GetFile();

	Long currentOffset = ftell(file);
	TCHAR character;
	size_t flag = fread(&character, 1, 1, file);
	while (character != '\n' && flag > 0 && !feof(file))
	{
		flag = fread(&character, 1, 1, file);
	}

	if (character != '\n')
	{
		fseek(file, currentOffset, SEEK_SET);
	}

	return ftell(file);
}

Long FilePointerMover::LastRow() {
	FILE* file = this->pagingBuffer->GetFile();

	//1. 파일의 끝으로 이동한다.
	fseek(file, 0, SEEK_END);
	Long fileEndOffset = ftell(file);

	//2. 마지막줄의 맨 앞으로 이동한다.
	TCHAR character[2];
	Long i = fileEndOffset - 1;
	int result = fseek(file, i, SEEK_SET);
	fread(character, 1, 1, file);
	while (i >= 0 && character[0] != '\n' && result == 0)
	{
		i--;
		result = fseek(file, i, SEEK_SET);
		fread(character, 1, 1, file);
	}

	if (result == 0) //3. 마지막 줄이 첫번째 줄이 아니면,
	{
		//3.1. 상단 적재되는 줄 수를 구한다.
		Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;
		Long newLineNeeded = loadingRowCount / 2 - 1;

		//3.2. 실제 적재되는 줄 수를 구한다.
		Long newLineCount = 0;
		Long offset = i;
		fseek(file, offset, SEEK_SET);
		fread(character, 1, 1, file);
		while (offset >= 0 && newLineCount < newLineNeeded)
		{
			fseek(file, offset, SEEK_SET);
			fread(character, 1, 1, file);
			if (character[0] == '\n')
			{
				newLineCount++;
			}
			offset--;
		}

		//3.3. 마지막 줄의 첫번째 위치로 이동한다.
		fseek(file, i + 1, SEEK_SET);
	}
	else //4. 마지막 줄이 첫번째 줄이라면,
	{
		//4.1. 파일의 시작 위치로 이동한다.
		fseek(file, 0, SEEK_SET);
	}

	return ftell(file);
}

Long FilePointerMover::MoveUpRows(Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	//1. 위쪽줄로 이동한다.
	TCHAR character;
	Long need;
	Long i;
	Long newLineCount = 0;
	need = count + 1;
	i = currentOffset - 1;
	while (i >= 0 && newLineCount < need)
	{
		fseek(file, i, SEEK_SET);
		fread(&character, 1, 1, file);

		if (character == '\n')
		{
			newLineCount++;
		}

		i--;
	}

	//2. 찾았으면,
	if (newLineCount >= need)
	{
		//2.1. 이동한다.
		i += 2;
	}
	else //3. 찾지 못했으면,
	{
		//3.1. 처음 위치로 이동한다.
		i = 0;
	}

	fseek(file, i, SEEK_SET);

	return ftell(file);
}

Long FilePointerMover::MoveDownRows(Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	//1. 아랫쪽 줄로 이동한다.
	TCHAR character;
	Long need = count;
	Long newLineCount = 0;
	size_t flag = fread(&character, 1, 1, file);
	while (newLineCount < need && flag > 0 && !feof(file))
	{
		if (character == '\n')
		{
			newLineCount++;
		}

		flag = fread(&character, 1, 1, file);
	}

	//2. 찾았으면,
	if (newLineCount >= need && !feof(file))
	{
		//2.1. 이동한다.
		fseek(file, -1, SEEK_CUR);
	}
	else //3. 찾지 못했으면,
	{
		//3.1. 마지막 줄의 시작으로 이동한다.
		Long i = ftell(file) - 1;
		fseek(file, i, SEEK_SET);
		fread(&character, 1, 1, file);
		while (i > currentOffset && character != '\n')
		{
			i--;
			fseek(file, i, SEEK_SET);
			fread(&character, 1, 1, file);
		}
	}

	return ftell(file);
}