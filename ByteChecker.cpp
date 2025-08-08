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