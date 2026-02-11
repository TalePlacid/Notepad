#ifndef _TEXTFILEIO_H //guard
#define _TEXTFILEIO_H

typedef signed long int Long;

class TextFileIO {
public:
	TextFileIO();
	~TextFileIO();

	Long Load(char(*path), char* (*bytes), Long& count);
	void Save(char(*path), char(*bytes), Long count);
};

#endif // !_TEXTFILEIO_H
