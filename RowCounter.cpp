#include "RowCounter.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

RowCounter::RowCounter() {

}

RowCounter::~RowCounter() {

}

Long RowCounter::CountRow(CString contents) {
	Long count = 0;

	Long i = 0;
	while (i < contents.GetLength())
	{
		if (contents[i] == '\n')
		{
			count++;
		}
		i++;
	}

	return count;
}

Long RowCounter::CountCharacters(CString row) {
	ByteChecker byteChecker;
	TCHAR character[2];
	Long count = 0;
	Long i = 0;
	while (i < row.GetLength())
	{
		character[0] = row[i];
		if (byteChecker.IsLeadByte(character[0]))
		{
			character[1] = row[++i];
		}
		count++;
		i++;
	}

	return count;
}