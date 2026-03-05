#include <afxwin.h>
#include "ByteChecker.h"

#pragma warning(disable:4996)

ByteChecker::ByteChecker() {

}

ByteChecker::~ByteChecker() {

}

bool ByteChecker::IsASCII(const char* character) {
	return static_cast<unsigned char>(*character) <= 0x7F;
}

bool ByteChecker::IsLeadByte(const char* firstByte, const char* secondByte) {
	//1. 첫 바이트가 리드 바이트 후보인지 판별한다.
	bool ret = false;
	const unsigned char firstByte_ = static_cast<const unsigned char>(*firstByte);
	const unsigned char secondByte_ = static_cast<const unsigned char>(*secondByte);

	BOOL isLeadCandidate = IsDBCSLeadByteEx(CP_ACP, (BYTE)firstByte_);

	//2. 리드 바이트 후보라면,
	if (isLeadCandidate)
	{
		//2.1. 두번째 바이트가 트레일 바이트 범위에 있는지 확인한다.
		if ((secondByte_ >= 0x41 && secondByte_ <= 0x5A)
			|| (secondByte_ >= 0x61 && secondByte_ <= 0x7A)
			|| (secondByte_ >= 0x81 && secondByte_ <= 0xFE))
		{
			ret = true;
		}
	}

	return ret;
}

bool ByteChecker::IsAlphabet(const char* character) {
	bool ret = false;
	unsigned char character_ = static_cast<unsigned char>(*character);

	if ((character_ >= 65 && character_ <= 90) // 소문자
		|| (character_ >= 97 && character_ <= 122)) //대문자
	{
		ret = true;
	}

	return ret;
}

bool ByteChecker::IsCapital(const char* character) {
	unsigned char character_ = static_cast<unsigned char>(*character);
	
	return character_ >= 65 && *character <= 90;
}

bool ByteChecker::IsWordCharacter(const char* character) {
	unsigned char character_ = static_cast<unsigned char>(*character);
	bool ret = false;

	if ((character_ >= 48 && character_ <= 57)
		|| (character_ >= 65 && character_ <= 90)
		|| (character_ >= 97 && character_ <= 122)) //숫자, 대문자, 소문자
	{
		ret = true;
	}
	else if (character_ < 0 || character_ > 127) // 비 ASCII문자
	{
		ret = true;
	}

	return ret;
}

Long ByteChecker::CheckUtf8FirstByte(const char* character) {
	Long bytes = 0;

	if ((*character & 0x80) == 0x00)
	{
		bytes = 1;
	}
	else if ((*character & 0xE0) == 0xC0)
	{
		bytes = 2;
	}
	else if ((*character & 0xF0) == 0xE0)
	{
		bytes = 3;
	}
	else if ((*character & 0xF8) == 0xF0)
	{
		bytes = 4;
	}

	return bytes;
}

bool ByteChecker::IsUtf8ContinuationByte(const char* character) {
	bool ret = false;

	if ((*character & 0xC0) == 0x80)
	{
		ret = true;
	}

	return ret;
}

