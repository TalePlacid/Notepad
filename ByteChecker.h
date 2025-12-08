#ifndef _BYTECHECKER_H //guard
#define _BYTECHECKER_H
#include <afxwin.h>

typedef signed long int Long;

class ByteChecker {
public:
	ByteChecker();
	~ByteChecker();

	bool IsASCII(char character);
	bool IsLeadByte(char character);
	bool IsTailByte(char character);
	bool IsCapital(char character);
	bool IsWordCharacter(char character);
};

#endif // !_BYTECHECKER_H
