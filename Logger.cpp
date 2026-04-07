#include <cstdio>
#include "Logger.h"
#include <shlobj.h>

#pragma warning(disable:4996)

Logger::Logger() {

}

Logger::~Logger() {

}

void Logger::Log(CString text) {
	//1. 바탕화면 경로를 얻는다.
    TCHAR path[MAX_PATH] = { 0 };
	SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, path);

	//2. 로그 파일 경로명을 만든다.
	CString logFilePath;
	logFilePath.Format("%s\\log.txt", path);
	
	//3. 로그를 적는다.
	FILE* file = fopen((LPCTSTR)logFilePath, "at");
	if (file != NULL)
	{
		fprintf(file, (LPCTSTR)text);
		fclose(file);
	}
}

void Logger::Clear() {
	//1. 바탕화면 경로를 얻는다.
	TCHAR path[MAX_PATH] = { 0 };
	SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, path);

	//2. 로그 파일 경로명을 만든다.
	CString logFilePath;
	logFilePath.Format("%s\\log.txt", path);

	//3. 로그 파일을 지운다.
	remove((LPCTSTR)logFilePath);
}