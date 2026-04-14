#include "TextFileIO.h"
#include "FileStreamAdapter.h"
#include "EncodingDetector.h"
#include "TextEncoder.h"
#include "LineBreakNormalizer.h"

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

	char(*encoded) = 0;
	Long encodedCount = 0;

	TextEncoder textEncoder;
	switch (encoding)
	{
	case UTF16LE:
		textEncoder.Utf16LeToAnsi(source, sourceCount, &encoded, encodedCount);
		break;
	case UTF16BE:
		textEncoder.Utf16BeToAnsi(source, sourceCount, &encoded, encodedCount);
		break;
	case UTF8BOM:
		textEncoder.Utf8BomToAnsi(source, sourceCount, &encoded, encodedCount);
		break;
	case UTF8:
		textEncoder.Utf8ToAnsi(source, sourceCount, &encoded, encodedCount);
		break;
	case ANSI:
		encoded = new char[sourceCount + 1];
		memcpy(encoded, source, sourceCount);
		encodedCount = sourceCount;
		encoded[encodedCount] = '\0';
		break;
	default:
		break;
	}

	LineBreakNormalizer::NormalizeLineBreak(encoded, encodedCount, bytes, count);

	if (encoded != 0)
	{
		delete[] encoded;
	}

	if (source != 0)
	{
		delete[] source;
	}

	return encoding;
}

Encoding TextFileIO::Save(const char(*path), const char(*bytes), Long count, Encoding encoding) {
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
