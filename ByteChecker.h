#ifndef _BYTECHECKER_H //guard
#define _BYTECHECKER_H

typedef signed long int Long;

class ByteChecker {
public:
	static bool IsASCII(const char* character);
	static bool IsLeadByteCandidate(const char* character);
	static bool IsTrailByteCandidate(const char* character);
	static bool IsAlphabet(const char* character);
	static bool IsCapital(const char* character);
	static bool IsWordCharacter(const char* character);
	static Long CheckUtf8FirstByte(const char* character);
	static bool IsUtf8ContinuationByte(const char* character);
};

#endif // !_BYTECHECKER_H