#include "ByteChecker.h"

#pragma warning(disable:4996)

ByteChecker::ByteChecker() {

}

ByteChecker::~ByteChecker() {

}

bool ByteChecker::IsASCII(char character) {
	return static_cast<unsigned char>(character) <= 0x7F;
}

bool ByteChecker::IsLeadByte(char character) {
	return static_cast<unsigned char>(character) & 0x80;
}

bool ByteChecker::IsCapital(char character) {
	unsigned char character_ = static_cast<unsigned char>(character);
	
	return character_ >= 65 && character <= 90;
}

bool ByteChecker::IsWordCharacter(char character) {
	unsigned char character_ = static_cast<unsigned char>(character);
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