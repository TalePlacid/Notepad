#ifndef _BYTECHECKER_H //guard
#define _BYTECHECKER_H

class ByteChecker {
public:
	ByteChecker();
	~ByteChecker();

	bool IsASCII(char character);
	bool IsLeadByte(char character);
	bool IsCapital(char character);
};

#endif // !_BYTECHECKER_H
