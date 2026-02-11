#include "TextFileIO.h"
#include "FileStreamAdapter.h"
#include "EncodingDetector.h"
#include "TextEncoder.h"

#pragma warning(disable:4996)

TextFileIO::TextFileIO() {

}

TextFileIO::~TextFileIO() {

}

Long TextFileIO::Load(char(*path), char* (*bytes), Long& count) {
	char(*source) = 0;
	Long sourceCount;

	FileStreamAdapter fileStreamAdapter;
	fileStreamAdapter.Read(path, &source, sourceCount);

	EncodingDetector encodingDetector;
	Encoding encoding = encodingDetector.ClassifyEncoding(source, sourceCount);

	TextEncoder textEncoder;
	switch (encoding)
	{
	case UTF16LE:
		textEncoder.Utf16LeToAnsi(source, sourceCount, bytes, count);
		break;
	case UTF16BE:
		textEncoder.Utf16BeToAnsi(source, sourceCount, bytes, count);
		break;
	case UTF8BOM:
		textEncoder.Utf8BomToAnsi(source, sourceCount, bytes, count);
		break;
	case UTF8:
		textEncoder.Utf8ToAnsi(source, sourceCount, bytes, count);
		break;
	case ANSI:
		(*bytes) = new char[sourceCount + 1];
		memcpy((*bytes), source, sourceCount);
		count = sourceCount;
		(*bytes)[count] = '\0';
		break;
	default:
		break;
	}

	if (source != 0)
	{
		delete[] source;
	}

	return count;
}