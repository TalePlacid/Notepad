#include "TextEncoder.h"

#pragma warning(disable:4996)

TextEncoder::TextEncoder() {

}

TextEncoder::~TextEncoder() {

}

void TextEncoder::Utf16LeToAnsi(const TCHAR(*source), Long sourceCount,
	TCHAR*(*encoded), Long& encodedCount) {
	Long bomCount = 2;
	Long characterCount = (sourceCount - bomCount) / 2;

	(*encoded) = new TCHAR[sourceCount];

	wchar_t(*source_) = (wchar_t*)(source);
	TCHAR defaultChar = '?';
	encodedCount = WideCharToMultiByte(CP_ACP, 0, source_ + 1, characterCount, 
		(*encoded), sourceCount, &defaultChar, NULL); // source_+1은 BOM 생략
	(*encoded)[encodedCount] = '\0';
}

void TextEncoder::Utf16BeToAnsi(const TCHAR(*source), Long sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//LE로 바이트 스왑
	TCHAR(*swapped) = new TCHAR[sourceCount];
	Long i = 0;
	while (i < sourceCount)
	{
		swapped[i] = source[i + 1];
		swapped[i + 1] = source[i];
		i += 2;
	}
	
	//LE로 인코딩
	this->Utf16LeToAnsi(swapped, sourceCount, encoded, encodedCount);

	if (swapped != NULL)
	{
		delete[] swapped;
	}
}

void TextEncoder::Utf8BomToAnsi(const TCHAR(*source), Long sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//1. BOM을 제거한다.
	Long bomCount = 3;
	Long sourceCount_ = sourceCount - 3;

	//2. UTF-8에서 ANSI로 변환한다.
	this->Utf8ToAnsi(source + bomCount, sourceCount_, encoded, encodedCount);
}

void TextEncoder::Utf8ToAnsi(const TCHAR(*source), Long sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//1. UTF16 LE로 변환한다.
	Long utf16Count = MultiByteToWideChar(CP_UTF8, 0, source, sourceCount, NULL, 0);
	wchar_t(*utf16Encoded) = new wchar_t[utf16Count + 1];
	utf16Count = MultiByteToWideChar(CP_UTF8, 0, source, sourceCount, utf16Encoded, utf16Count);
	utf16Encoded[utf16Count] = L'\0';

	//2. UTF16 LE를 ANSI로 변환한다.
	Long maxSize = utf16Count * 2;
	(*encoded) = new TCHAR[maxSize + 1];

	TCHAR defaultChar = '?';
	encodedCount = WideCharToMultiByte(CP_ACP, 0, utf16Encoded, utf16Count,
		(*encoded), maxSize, &defaultChar, NULL);
	(*encoded)[encodedCount] = '\0';

	if (utf16Encoded != NULL)
	{
		delete[] utf16Encoded;
	}
}

void TextEncoder::AnsiToUtf16Le(const TCHAR(*source), Long sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//1. UTF16크기만큼 생성한다.
	Long utf16Count = MultiByteToWideChar(CP_ACP, 0, source, sourceCount, NULL, 0);
	(*encoded) = (TCHAR*)new wchar_t[utf16Count + 1];

	//2. BOM을 새긴다.
	(*encoded)[0] = 0xFF;
	(*encoded)[1] = 0xFE;
	Long bomCount = 2;

	//3. ANSI를 UTF-16 LE로 변환한다.
	wchar_t(*encoded_) = (wchar_t*)((*encoded) + bomCount);
	utf16Count = MultiByteToWideChar(CP_ACP, 0, source, sourceCount, encoded_, utf16Count);
	encodedCount = (utf16Count + 1) * 2;
}

void TextEncoder::AnsiToUtf16Be(const TCHAR(*source), Long sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//1. UTF-16 LE형식으로 변환한다.
	TCHAR(*leEncoded) = NULL;
	Long leCount = 0;
	this->AnsiToUtf16Le(source, sourceCount, &leEncoded, leCount);

	//2. BE 형식으로 바이트 스왑한다.
	encodedCount = leCount;
	(*encoded) = new TCHAR[encodedCount];
	
	Long i = 0;
	while (i < encodedCount)
	{
		(*encoded)[i] = leEncoded[i + 1];
		(*encoded)[i + 1] = leEncoded[i];
		i += 2;
	}

	if (leEncoded != NULL)
	{
		delete[] leEncoded;
	}
}

void TextEncoder::AnsiToUtf8Bom(const TCHAR(*source), Long sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//1. UTF16 LE형식으로 변환한다.
	Long leCount = MultiByteToWideChar(CP_ACP, 0, source, sourceCount, NULL, 0);
	wchar_t(*leEncoded) = new wchar_t[leCount];
	leCount = MultiByteToWideChar(CP_ACP, 0, source, sourceCount, leEncoded, leCount);

	//2. UTF-8 개수를 구한다.
	TCHAR defaultCharacter = '?';
	Long utf8Count = WideCharToMultiByte(CP_UTF8, 0, leEncoded, leCount, NULL, 0, &defaultCharacter, NULL);
	
	//3. BOM을 새긴다.
	(*encoded) = new TCHAR[utf8Count + 3];
	(*encoded)[0] = 0xEF;
	(*encoded)[1] = 0xBB;
	(*encoded)[2] = 0xBF;

	//4. UTF-8으로 변환한다.
	WideCharToMultiByte(CP_UTF8, 0, leEncoded, leCount,
		(*encoded) + 3, utf8Count, &defaultCharacter, NULL);
	encodedCount = utf8Count + 3;

	if (leEncoded != NULL)
	{
		delete[] leEncoded;
	}
}

void TextEncoder::AnsiToUtf8(const TCHAR(*source), Long sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//1. UTF16 LE형식으로 변환한다.
	Long leCount = MultiByteToWideChar(CP_ACP, 0, source, sourceCount, NULL, 0);
	wchar_t(*leEncoded) = new wchar_t[leCount];
	leCount = MultiByteToWideChar(CP_ACP, 0, source, sourceCount, leEncoded, leCount);

	//2. UTF-8 개수를 구한다.
	TCHAR defaultCharacter = '?';
	encodedCount = WideCharToMultiByte(CP_UTF8, 0, leEncoded, leCount, NULL, 0, &defaultCharacter, NULL);

	(*encoded) = new TCHAR[encodedCount];

	//3. UTF-8으로 변환한다.
	encodedCount = WideCharToMultiByte(CP_UTF8, 0, leEncoded, leCount,
		(*encoded), encodedCount, &defaultCharacter, NULL);

	if (leEncoded != NULL)
	{
		delete[] leEncoded;
	}
}
