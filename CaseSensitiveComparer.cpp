#include "CaseSensitiveComparer.h"

#pragma warning(disable:4996)

CaseSensitiveComparer::CaseSensitiveComparer() {

}

CaseSensitiveComparer::~CaseSensitiveComparer() {

}

bool CaseSensitiveComparer::Compare(char& one, char& other) {
	return one == other;
}