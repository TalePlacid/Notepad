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