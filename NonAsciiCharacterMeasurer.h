#ifndef _NONASCIICHARACTERMEASURER_H
#define _NONASCIICHARACTERMEASURER_H

typedef signed long int Long;

class CWnd;

class NonAsciiCharacterMeasurer {
public:
	NonAsciiCharacterMeasurer();
	~NonAsciiCharacterMeasurer();

	static Long MesureWidth(CWnd* parent, char(*character));
};

#endif // !_NONASCIICHARACTERMEASURER_H
