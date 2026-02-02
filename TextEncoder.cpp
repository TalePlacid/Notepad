#include "TextEncoder.h"

#pragma warning(disable:4996)

TextEncoder::TextEncoder() {

}

TextEncoder::~TextEncoder() {

}

void TextEncoder::Utf16LEToAnsi(TCHAR(*source), Long& sourceCount,
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

void TextEncoder::Utf16BEToAnsi(TCHAR(*source), Long& sourceCount,
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
	this->Utf16LEToAnsi(swapped, sourceCount, encoded, encodedCount);

	if (swapped != NULL)
	{
		delete[] swapped;
	}
}

void TextEncoder::Utf8BomToAnsi(TCHAR(*source), Long& sourceCount,
	TCHAR* (*encoded), Long& encodedCount) {
	//1. BOM을 제거한다.
	Long bomCount = 3;
	Long sourceCount_ = sourceCount - 3;

	//2. UTF-8에서 ANSI로 변환한다.
	this->Utf8ToAnsi(source + bomCount, sourceCount_, encoded, encodedCount);
}

void TextEncoder::Utf8ToAnsi(TCHAR(*source), Long& sourceCount,
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