#ifndef _TEXTENCODER_H //guard
#define _TEXTENCODER_H
#include <afxwin.h>

typedef signed long int Long;

class TextEncoder {
public:
	TextEncoder();
	~TextEncoder();

	void Utf16LeToAnsi(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf16BeToAnsi(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf8BomToAnsi(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf8ToAnsi(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);

	void AnsiToUtf16Le(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void AnsiToUtf16Be(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void AnsiToUtf8Bom(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void AnsiToUtf8(TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
};

#endif // !_TEXTENCODER_H
