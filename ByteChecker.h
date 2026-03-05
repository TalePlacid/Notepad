#ifndef _BYTECHECKER_H //guard
#define _BYTECHECKER_H

typedef signed long int Long;

class ByteChecker {
public:
	ByteChecker();
	~ByteChecker();

	bool IsASCII(const char* character);
	bool IsLeadByte(const char* firstByte, const char* secondByte);
	bool IsAlphabet(const char* character);
	bool IsCapital(const char* character);
	bool IsWordCharacter(const char* character);
	Long CheckUtf8FirstByte(const char* character);
	bool IsUtf8ContinuationByte(const char* character);
};

#endif // !_BYTECHECKER_H

