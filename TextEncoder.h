#ifndef _TEXTENCODER_H //guard
#define _TEXTENCODER_H
#include <afxwin.h>

typedef signed long int Long;

class TextEncoder {
public:
	TextEncoder();
	~TextEncoder();

	void Utf16LEToAnsi(TCHAR(*source), Long& sourceCount, TCHAR*(*encoded), Long& encodedCount);
	void Utf16BEToAnsi(TCHAR(*source), Long& sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf8BomToAnsi(TCHAR(*source), Long& sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf8ToAnsi(TCHAR(*source), Long& sourceCount, TCHAR* (*encoded), Long& encodedCount);
};

#endif // !_TEXTENCODER_H
