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

		//3.3. ������ ���� ù��° ��ġ�� �̵��Ѵ�.
		fseek(file, i + 1, SEEK_SET);
	}
	else //4. ������ ���� ù��° ���̶��,
	{
		//4.1. ������ ���� ��ġ�� �̵��Ѵ�.
		fseek(file, 0, SEEK_SET);
	}

	return ftell(file);
}

Long FilePointerMover::MoveUpRows(Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	//1. �����ٷ� �̵��Ѵ�.
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

	//2. ã������,
	if (newLineCount >= need)
	{
		//2.1. �̵��Ѵ�.
		i += 2;
	}
	else //3. ã�� ��������,
	{
		//3.1. ó�� ��ġ�� �̵��Ѵ�.
		i = 0;
	}

	fseek(file, i, SEEK_SET);

	return ftell(file);
}

Long FilePointerMover::MoveDownRows(Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	//1. �Ʒ��� �ٷ� �̵��Ѵ�.
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

	return ftell(file);
}