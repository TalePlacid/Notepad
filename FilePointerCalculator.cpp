#include "FilePointerCalculator.h"
#include "PagingBuffer.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

FilePointerCalculator::FilePointerCalculator(PagingBuffer* pagingBuffer) {
	this->pagingBuffer = pagingBuffer;
}

FilePointerCalculator::~FilePointerCalculator() {

}

Long FilePointerCalculator::First(Long offset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	TCHAR character;
	Long i = offset - 1;
	int result = fseek(file, i, SEEK_SET);
	fread(&character, 1, 1, file);
	while (i >= 0 && character != '\n' && result == 0)
	{
		i--;
		result = fseek(file, i, SEEK_SET);
		fread(&character, 1, 1, file);
	}

	Long firstOffset = i + 1;
	if (result != 0)
	{
		firstOffset = 0;
	}

	fseek(file, currentOffset, SEEK_SET);

	return firstOffset;
}

Long FilePointerCalculator::Previous(Long offset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	Long previousOffset = 0;
	if (offset > 0)
	{
		TCHAR character;
		fseek(file, offset - 1, SEEK_SET);
		fread(&character, 1, 1, file);

		ByteChecker byteChecker;
		if (!byteChecker.IsASCII(character))
		{
			previousOffset = offset - 2;
		}
		else if (character != '\n')
		{
			previousOffset = offset - 1;
		}
	}

	fseek(file, currentOffset, SEEK_SET);

	return previousOffset;
}

Long FilePointerCalculator::Next(Long offset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	TCHAR character[2];
	Long nextOffset = offset;
	fseek(file, offset, SEEK_SET);
	size_t flag = fread(character, 1, 1, file);
	if (flag > 0 && !feof(file))
	{
		if (character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, file);
			nextOffset = offset + 2;
		}
		else if (character[0] != '\r')
		{
			nextOffset = offset + 1;
		}
	}

	fseek(file, currentOffset, SEEK_SET);

	return nextOffset;
}

Long FilePointerCalculator::Last(Long offset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	TCHAR character = '\0';
	fseek(file, offset, SEEK_SET);
	size_t flag = fread(&character, 1, 1, file);
	while (character != '\r' && flag > 0 && !feof(file))
	{
		flag = fread(&character, 1, 1, file);
	}

	Long lastOffset = ftell(file);
	if (character == '\r')
	{
		lastOffset--;
	}

	fseek(file, currentOffset, SEEK_SET);

	return lastOffset;
}

Long FilePointerCalculator::Move(Long offset, Long index) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	TCHAR character[2];
	Long i = offset - 1;
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

	Long movedOffset = ftell(file);

	fseek(file, currentOffset, SEEK_SET);

	return movedOffset;
}

Long FilePointerCalculator::FirstRow() {
	return 0;
}

Long FilePointerCalculator::PreviousRow(Long offset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	TCHAR character[2];
	Long count = 0;
	Long i = offset - 1;
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

	Long previousRowOffset = ftell(file);
	if (count == 1)
	{
		previousRowOffset = 0;
	}
	else if (count == 0)
	{
		previousRowOffset = offset;
	}

	fseek(file, currentOffset, SEEK_SET);

	return previousRowOffset;
}

Long FilePointerCalculator::NextRow(Long offset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);
	
	fseek(file, offset, SEEK_SET);
	TCHAR character;
	size_t flag = fread(&character, 1, 1, file);
	while (character != '\n' && flag > 0 && !feof(file))
	{
		flag = fread(&character, 1, 1, file);
	}

	Long nextRowOffset = ftell(file);
	if (character != '\n')
	{
		nextRowOffset = offset;
	}

	fseek(file, currentOffset, SEEK_SET);

	return nextRowOffset;
}

Long FilePointerCalculator::LastRow() {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	//1. ������ ������ �̵��Ѵ�.
	fseek(file, 0, SEEK_END);
	Long fileEndOffset = ftell(file);

	//2. ���������� �� ������ �̵��Ѵ�.
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

	Long lastRowOffset;
	if (result == 0) //3. ������ ���� ù��° ���� �ƴϸ�,
	{
		//3.1. ��� ����Ǵ� �� ���� ���Ѵ�.
		Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;
		Long newLineNeeded = loadingRowCount / 2 - 1;

		//3.2. ���� ����Ǵ� �� ���� ���Ѵ�.
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

		//3.3. ������ ���� ù��° ��ġ�� ���Ѵ�.
		lastRowOffset = i + 1;
	}
	else //4. ������ ���� ù��° ���̶��,
	{
		//4.1. ������ ���� ��ġ�� ���Ѵ�.
		lastRowOffset = 0;
	}

	fseek(file, currentOffset, SEEK_SET);

	return lastRowOffset;
}

Long FilePointerCalculator::MoveUpRows(Long offset, Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	//1. �����ٷ� �̵��Ѵ�.
	fseek(file, offset, SEEK_SET);
	TCHAR character;
	Long need;
	Long i;
	Long newLineCount = 0;
	need = count + 1;
	i = offset - 1;
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

	//2. ã������,
	Long upRowOffset;
	if (newLineCount >= need)
	{
		//2.1. �̵��Ѵ�.
		upRowOffset = i + 2;
	}
	else //3. ã�� ��������,
	{
		//3.1. ó�� ��ġ�� �̵��Ѵ�.
		upRowOffset = 0;
	}

	fseek(file, currentOffset, SEEK_SET);

	return upRowOffset;
}

Long FilePointerCalculator::MoveDownRows(Long offset, Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	//1. �Ʒ��� �ٷ� �̵��Ѵ�.
	fseek(file, offset, SEEK_SET);
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

	//2. ã������,
	if (newLineCount >= need && !feof(file))
	{
		//2.1. �̵��Ѵ�.
		fseek(file, -1, SEEK_CUR);
	}
	else //3. ã�� ��������,
	{
		//3.1. ������ ���� �������� �̵��Ѵ�.
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

	Long downRowOffset = ftell(file);

	fseek(file, currentOffset, SEEK_SET);

	return downRowOffset;
}

Long FilePointerCalculator::FileEnd() {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	fseek(file, 0, SEEK_END);
	Long fileEndOffset = ftell(file);

	fseek(file, currentOffset, SEEK_SET);

	return fileEndOffset;
}