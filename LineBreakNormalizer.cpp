#include "LineBreakNormalizer.h"

#pragma warning(disable:4996)

LineBreakNormalizer::LineBreakNormalizer() {

}

LineBreakNormalizer::~LineBreakNormalizer() {

}

void LineBreakNormalizer::NormalizeLineBreak(const TCHAR(*source), const Long sourceCount,
	TCHAR* (*normalized), Long& normalizedCount) {
	//1. LF를 탐색한다.
	Long(*lfIndexes) = new Long[sourceCount + 1];
	Long lfCount = 0;
	Long i = 0;
	
	if (sourceCount > 0 && source[0] == '\n')
	{
		lfIndexes[i] = 0;
		i++;
		lfCount++;
	}

	Long j = 1;
	while (j < sourceCount)
	{
		if (source[j] == '\n' && source[j-1] != '\r')
		{
			lfIndexes[i] = j;
			i++;
			lfCount++;
		}
		j++;
	}

	//2. 정규화된 문자열을 만든다.
	(*normalized) = new TCHAR[sourceCount + lfCount + 1];
	normalizedCount = sourceCount + lfCount;

	//3. 문자열을 정규화한다.
	Long k = 0;
	j = 0;
	while (j < lfCount)
	{
		while (k < lfIndexes[j] + j)
		{
			(*normalized)[k] = source[k - j];
			k++;
		}

		(*normalized)[k] = '\r';
		k++;

		j++;
	}

	while (k < sourceCount + lfCount)
	{
		(*normalized)[k] = source[k - lfCount];
		k++;
	}

	(*normalized)[normalizedCount] = '\0';

	if (lfIndexes != NULL)
	{
		delete[] lfIndexes;
	}
}