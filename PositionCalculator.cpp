#include "PositionCalculator.h"
#include "PagingBuffer.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

PositionCalculator::PositionCalculator(PagingBuffer* pagingBuffer) {
	this->pagingBuffer = pagingBuffer;
}

PositionCalculator::~PositionCalculator() {

}

Position PositionCalculator::First(Position position) {
	return position.Move(position.GetRow(), 0);
}

Position PositionCalculator::Previous(Position position) {
	Position previous = position;
	if (previous.GetColumn() > 0)
	{
		previous = previous.Left();
	}

	return previous;
}

Position PositionCalculator::Next(Position position, Long oldOffset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	TCHAR character;
	Position next = position;
	fseek(file, oldOffset, SEEK_SET);
	size_t flag = fread(&character, 1, 1, file);
	if (character != '\r' && flag > 0 && !feof(file))
	{
		next = next.Right();
	}

	fseek(file, currentOffset, SEEK_SET);

	return next;
}

Position PositionCalculator::Last(Position position, Long oldOffset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = this->pagingBuffer->GetCurrentOffset();

	ByteChecker byteChecker;
	TCHAR character[2];
	Position last = position;
	fseek(file, oldOffset, SEEK_SET);
	size_t flag = fread(character, 1, 1, file);
	while (character[0] != '\r' && flag > 0 && !feof(file))
	{
		if (byteChecker.IsLeadByte(character[0]))
		{
			flag = fread(character, 1, 1, file);
		}

		last = last.Right();

		flag = fread(character, 1, 1, file);
	}

	fseek(file, currentOffset, SEEK_SET);

	return last;
}

Position PositionCalculator::Move(Position position, Long index) {
	return position.Move(position.GetRow(), index);
}

Position PositionCalculator::FirstRow() {
	return Position(0, 0);
}

Position PositionCalculator::PreviousRow(Position position, Long oldOffset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	TCHAR character[2];
	Position previousRowPosition = position;
	Long count = 0;
	Long i = oldOffset - 1;
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
		previousRowPosition = previousRowPosition.Up();
		previousRowPosition = previousRowPosition.Move(previousRowPosition.GetRow(), 0);
	}

	fseek(file, currentOffset, SEEK_SET);

	return previousRowPosition;
}

Position PositionCalculator::NextRow(Position position, Long oldOffset) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	Position nextRowPosition = position;
	fseek(file, oldOffset, SEEK_SET);
	TCHAR character;
	size_t flag = fread(&character, 1, 1, file);
	while (character != '\n' && flag > 0 && !feof(file))
	{
		flag = fread(&character, 1, 1, file);
	}

	if (character == '\n')
	{
		nextRowPosition = nextRowPosition.Down();
		nextRowPosition = nextRowPosition.Move(nextRowPosition.GetRow(), 0);
	}

	fseek(file, currentOffset, SEEK_SET);

	return nextRowPosition;
}

Position PositionCalculator::LastRow() {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	fseek(file, 0, SEEK_END);
	Long fileEndOffset = ftell(file);

	TCHAR character[2];
	Long i = fileEndOffset - 1;
	fseek(file, i, SEEK_SET);
	fread(character, 1, 1, file);
	if (i >= 0 && character[0] != '\n')
	{
		i--;
		fseek(file, i, SEEK_SET);
		fread(character, 1, 1, file);
	}

	Position lastRowPosition;
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
		lastRowPosition = Position(newLineCount, 0);
	}
	else //4. 마지막 줄이 첫번째 줄이라면,
	{
		//4.1. 파일의 시작 위치로 이동한다.
		lastRowPosition = Position(0, 0);
	}

	fseek(file, currentOffset, SEEK_SET);

	return lastRowPosition;
}

Position PositionCalculator::MoveUpRows(Position position, Long oldOffset, Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);
	
	Position upRowPosition = position;
	TCHAR character;
	Long newLineCount = 0;
	Long i = oldOffset - 1;
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
		upRowPosition = upRowPosition.Up(count);
	}
	else
	{
		upRowPosition = upRowPosition.Up(newLineCount);
	}
	upRowPosition = upRowPosition.Move(upRowPosition.GetRow(), 0);

	fseek(file, currentOffset, SEEK_SET);

	return upRowPosition;
}

Position PositionCalculator::MoveDownRows(Position position, Long oldOffset, Long count) {
	FILE* file = this->pagingBuffer->GetFile();
	Long currentOffset = ftell(file);

	Position downRowPosition = position;
	TCHAR character;
	Long newLineCount = 0;
	fseek(file, oldOffset, SEEK_SET);
	size_t flag = fread(&character, 1, 1, file);
	while (flag > 0 && !feof(file) && newLineCount < count)
	{
		if (character == '\n')
		{
			newLineCount++;
		}

		flag = fread(&character, 1, 1, file);
	}

	if (newLineCount >= count)
	{
		downRowPosition = downRowPosition.Down(count);
	}
	else
	{
		downRowPosition = downRowPosition.Down(newLineCount);
	}
	downRowPosition = downRowPosition.Move(downRowPosition.GetRow(), 0);

	fseek(file, currentOffset, SEEK_SET);

	return downRowPosition;
}