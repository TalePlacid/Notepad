#include <afxwin.h>
#include "ByteChecker.h"

#pragma warning(disable:4996)

bool ByteChecker::IsASCII(const char* character) {
	return static_cast<unsigned char>(*character) <= 0x7F;
}

bool ByteChecker::IsLeadByteCandidate(const char* character) {
	const unsigned char checkingByte = static_cast<const unsigned char>(*character);

	return IsDBCSLeadByteEx(CP_ACP, (BYTE)checkingByte) ? true : false;
}

bool ByteChecker::IsTrailByteCandidate(const char* character) {
	bool ret = false;
	const unsigned char checkingByte = static_cast<const unsigned char>(*character);

	if ((checkingByte >= 0x41 && checkingByte <= 0x5A)
		|| (checkingByte >= 0x61 && checkingByte <= 0x7A)
		|| (checkingByte >= 0x81 && checkingByte <= 0xFE))
	{
		ret = true;
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