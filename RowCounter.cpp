#include "RowCounter.h"

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