#include "CaseInsensitiveComparer.h"
#include "../ByteChecker.h"

#pragma warning(disable:4996)

CaseInsensitiveComparer::CaseInsensitiveComparer() {

}

CaseInsensitiveComparer::~CaseInsensitiveComparer() {

}

bool CaseInsensitiveComparer::Compare(char& one, char& other) {
	char one_ = one;
	char other_ = other;
	
	
	if (ByteChecker::IsCapital(&one_))
	{
		one_ += 32;
	}

	if (ByteChecker::IsCapital(&other_))
	{
		other_ += 32;
	}

	return one_ == other_;
}

