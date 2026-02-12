#include "TextFileIO.h"
#include "FileStreamAdapter.h"
#include "EncodingDetector.h"
#include "TextEncoder.h"

#pragma warning(disable:4996)

TextFileIO::TextFileIO() {

}

TextFileIO::~TextFileIO() {

}

Encoding TextFileIO::Load(const char(*path), char* (*bytes), Long& count) {
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

	return encoding;
}

Encoding TextFileIO::Save(const char(*path), const char(*bytes), Long count) {
	EncodingDetector encodingDetector;
	Encoding encoding = encodingDetector.ClassifyEncoding(bytes, count);

	TCHAR(*encoded) = NULL;
	Long encodedCount;
	TextEncoder textEncoder;
	switch (encoding)
	{
	case UTF16LE:
		textEncoder.AnsiToUtf16Le(bytes, count, &encoded, encodedCount);
		break;
	case UTF16BE:
		textEncoder.AnsiToUtf16Be(bytes, count, &encoded, encodedCount);
		break;
	case UTF8BOM:
		textEncoder.AnsiToUtf8Bom(bytes, count, &encoded, encodedCount);
		break;
	case UTF8:
		textEncoder.AnsiToUtf8(bytes, count, &encoded, encodedCount);
		break;
	case ANSI:
		encoded = new TCHAR[count + 1];
		memcpy(encoded, bytes, count);
		encodedCount = count;
		encoded[encodedCount] = '\0';
		break;
	default:
		break;
	}

	FileStreamAdapter fileStreamAdapter;
	fileStreamAdapter.Write(path, encoded, encodedCount);

	if (encoded != NULL)
	{
		delete[] encoded;
	}

	return encoding;
}