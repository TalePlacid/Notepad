#ifndef _LEADBYTECHECKER_H //guard
#define _LEADBYTECHECKER_H

#include <cstdio>

typedef signed long int Long;

class LeadByteChecker {
public:
	LeadByteChecker();
	~LeadByteChecker();

	static bool IsLeadByte(FILE* file, Long offset);
};

#endif // !_LEADBYTECHECKER_H