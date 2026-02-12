#include <cstdio>
#include "FileStreamAdapter.h"

#pragma warning(disable:4996)

FileStreamAdapter::FileStreamAdapter() {

}

FileStreamAdapter::~FileStreamAdapter() {

}

Long FileStreamAdapter::Read(const char(*path), char*(*bytes), Long& count) {
	count = 0;

	FILE* file = fopen(path, "rb");
	if (file != 0)
	{
		fseek(file, 0, SEEK_END);
		Long fileEndOffset = ftell(file);
		(*bytes) = new char[fileEndOffset + 1];

		fseek(file, 0, SEEK_SET);
		count = fread((*bytes), 1, fileEndOffset, file);
		(*bytes)[count] = '\0';

		fclose(file);
	}

	return count;
}

void FileStreamAdapter::Write(const char(*path), char(*bytes), Long count) {
	FILE* file = fopen(path, "wb");

	if (file != 0)
	{
		fwrite(bytes, 1, count, file);
		fclose(file);
	}
}