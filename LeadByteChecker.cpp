#include <afxwin.h>
#include "LeadByteChecker.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

LeadByteChecker::LeadByteChecker() {

}

LeadByteChecker::~LeadByteChecker() {

}

bool LeadByteChecker::IsLeadByte(FILE* file, Long offset) {
	//1. 현재 파일 위치를 보관한다.
	bool ret = false;
	Long currentOffset = ftell(file);

	//2. 현재 위치에서 줄 시작 또는 문서 시작까지 되돌아간다.
	Long i = offset;
	char previousByte = '\0';
	while (i > 0 && previousByte != '\n')
	{
		fseek(file, i - 1, SEEK_SET);
		fread(&previousByte, 1, 1, file);
		if (previousByte != '\n')
		{
			i--;
		}
	}

	//3. 줄 시작부터 현재 위치까지 문자 단위로 전진한다.
	while (i < offset)
	{
		char checkingByte;
		fseek(file, i, SEEK_SET);
		fread(&checkingByte, 1, 1, file);
		if (ByteChecker::IsASCII(&checkingByte))
		{
			i += 1;
		}
		else
		{
			i += 2;
		}
	}

	//4. 현재 위치가 문자 시작점이면 현재 바이트로 리드 바이트 여부를 판정한다.
	if (i == offset)
	{
		char checkingByte;
		fseek(file, i, SEEK_SET);
		fread(&checkingByte, 1, 1, file);
		if (!ByteChecker::IsASCII(&checkingByte))
		{
			ret = true;
		}
	}

	//5. 원래 파일 위치를 복원한다.
	fseek(file, currentOffset, SEEK_SET);

	return ret;
}