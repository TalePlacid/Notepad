#ifndef _TEXTFILEIO_H //guard
#define _TEXTFILEIO_H
#include "Encoding.h"

typedef signed long int Long;

class TextFileIO {
public:
	TextFileIO();
	~TextFileIO();

	Encoding Load(const char(*path), char* (*bytes), Long& count);
	Encoding Save(const char(*path), const char(*bytes), Long count);
};

#endif // !_TEXTFILEIO_H

