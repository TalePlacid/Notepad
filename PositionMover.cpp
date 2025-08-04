#include "PositionMover.h"
#include "PagingBuffer.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

PositionMover::PositionMover(PagingBuffer *pagingBuffer) {
	this->pagingBuffer = pagingBuffer;
}

PositionMover::~PositionMover() {

}

Position PositionMover::First() {
	Position current = this->pagingBuffer->GetCurrent();

	return current.Move(current.GetRow(), 0);
}

Position PositionMover::Previous(Long oldCurrentOffset) {
	FILE* file = this->pagingBuffer->GetFile();
	Position current = this->pagingBuffer->GetCurrent();
	Long currentOffset = ftell(file);
	if (oldCurrentOffset > 0)
	{
		TCHAR character;
		fseek(file, oldCurrentOffset - 1, SEEK_SET);
		fread(&character, 1, 1, file);

		if (character != '\n')
		{
			current = current.Left();
		}

		fseek(file, currentOffset, SEEK_SET);
	}

	return current;
}

Position PositionMover::Next(Long oldCurrentOffset) {
	FILE* file = this->pagingBuffer->GetFile();
	Position current = this->pagingBuffer->GetCurrent();
	Long currentOffset = ftell(file);
	
	TCHAR character;
	fseek(file, oldCurrentOffset, SEEK_SET);
	size_t flag = fread(&character, 1, 1, file);
	if (character != '\r' && flag > 0)
	{
		current = current.Right();
	}

	fseek(file, currentOffset, SEEK_SET);

	return current;
}

Position PositionMover::Last(Long oldCurrentOffset) {
	FILE* file = this->pagingBuffer->GetFile();

	Position current = this->pagingBuffer->GetCurrent();
	Long currentOffset = this->pagingBuffer->GetCurrentOffset();

	ByteChecker byteChecker;
	TCHAR character[2];
	Long i = oldCurrentOffset;
	fseek(file, i, SEEK_SET);
	while (i < currentOffset)
	{
		fread(character, 1, 1, file);
		i++;

		if (byteChecker.IsLeadByte(character[0]))
		{
			fread(character + 1, 1, 1, file);
			i++;
		}

		current = current.Right();
	}

	fseek(file, currentOffset, SEEK_SET);

	return current;
}

Position PositionMover::Move(Long index) {
	Position current = this->pagingBuffer->GetCurrent();

	return current.Move(current.GetRow(), index);
}

Position PositionMover::FirstRow() {
	return Position(0, 0);
}

Position PositionMover::PreviousRow(Long oldCurrentOffset) {
	FILE* file = this->pagingBuffer->GetFile();

	Long currentOffset = ftell(file);
	TCHAR character[2];
	Position current = this->pagingBuffer->GetCurrent();
	Long count = 0;
	Long i = oldCurrentOffset - 1;
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

	if (count > 0)
	{
		current = current.Up();
		current = current.Move(current.GetRow(), 0);
	}

	fseek(file, currentOffset, SEEK_SET);

	return current;
}

Position PositionMover::NextRow(Long oldCurrentOffset) {
	FILE* file = this->pagingBuffer->GetFile();
	Position current = this->pagingBuffer->GetCurrent();
	Long currentOffset = ftell(file);

	fseek(file, oldCurrentOffset, SEEK_SET);
	TCHAR character;
	size_t flag = fread(&character, 1, 1, file);
	while (character != '\n' && flag > 0 && !feof(file))
	{
		flag = fread(&character, 1, 1, file);
	}

	if (character == '\n')
	{
		current = current.Down();
		current = current.Move(current.GetRow(), 0);
	}

	fseek(file, currentOffset, SEEK_SET);

	return current;
}

Position PositionMover::LastRow() {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	Position current;
	Long i = currentOffset - 1;
	if (i >= 0) //1. 마지막 줄이 첫번째 줄이 아니면,
	{
		//1.1. 상단 적재되는 줄 수를 구한다.
		Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;
		Long newLineNeeded = loadingRowCount / 2 - 1;

		//1.2. 실제 적재되는 줄 수를 구한다.
		TCHAR character;
		Long newLineCount = 0;
		Long offset = i;
		fseek(file, offset, SEEK_SET);
		fread(&character, 1, 1, file);
		while (offset >= 0 && newLineCount < newLineNeeded)
		{
			fseek(file, offset, SEEK_SET);
			fread(&character, 1, 1, file);
			if (character == '\n')
			{
				newLineCount++;
			}
			offset--;
		}

		//3.3. 마지막 줄의 첫번째 위치로 이동한다.
		current = Position(newLineCount, 0);
	}
	else //4. 마지막 줄이 첫번째 줄이라면,
	{
		//4.1. 파일의 시작 위치로 이동한다.
		current = Position(0, 0);
	}

	fseek(file, currentOffset, SEEK_SET);

	return current;
}

Position PositionMover::MoveUpRows(Long oldCurrentOffset, Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);
	Position current = this->pagingBuffer->GetCurrent();

	TCHAR character;
	Long newLineCount = 0;
	Long i = oldCurrentOffset - 1;
	while (i >= 0 && newLineCount < count)
	{
		fseek(file, i, SEEK_SET);
		fread(&character, 1, 1, file);

		if (character == '\n')
		{
			newLineCount++;
		}

		i--;
	}

	if (newLineCount >= count)
	{
		current = current.Up(count);
	}
	else
	{
		current = current.Up(newLineCount);
	}
	current = current.Move(current.GetRow(), 0);

	fseek(file, currentOffset, SEEK_SET);

	return current;
}

Position PositionMover::MoveDownRows(Long oldCurrentoffset, Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);
	Position current = this->pagingBuffer->GetCurrent();

	TCHAR character;
	Long newLineCount = 0;
	Long i = oldCurrentoffset + 1;
	fseek(file, oldCurrentoffset, SEEK_SET);
	size_t flag = fread(&character, 1, 1, file);
	while (flag > 0 && !feof(file) && newLineCount < count)
	{
		if (character == '\n')
		{
			newLineCount++;
		}

		i++;
		flag = fread(&character, 1, 1, file);
	}

	if (newLineCount >= count)
	{
		current = current.Down(count);
	}
	else
	{
		current = current.Down(newLineCount);
	}
	current = current.Move(current.GetRow(), 0);

	fseek(file, currentOffset, SEEK_SET);

	return current;
}