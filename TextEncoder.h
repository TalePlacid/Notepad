#ifndef _TEXTENCODER_H //guard
#define _TEXTENCODER_H
#include <afxwin.h>

typedef signed long int Long;

class TextEncoder {
public:
	TextEncoder();
	~TextEncoder();

	void Utf16LeToAnsi(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf16BeToAnsi(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf8BomToAnsi(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void Utf8ToAnsi(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);

	void AnsiToUtf16Le(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void AnsiToUtf16Be(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void AnsiToUtf8Bom(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
	void AnsiToUtf8(const TCHAR(*source), Long sourceCount, TCHAR* (*encoded), Long& encodedCount);
};

#endif // !_TEXTENCODER_H
