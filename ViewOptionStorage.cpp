#include <cstdio>
#include "ViewOptionStorage.h"

#pragma warning(disable:4996)

ViewOptionStorage::ViewOptionStorage() {

}

ViewOptionStorage::~ViewOptionStorage() {

}

BOOL ViewOptionStorage::Load(LOGFONT& logFont, BOOL& isAutoWrapped, double& magnification, BOOL& isStatusBarVisible) {
	//1. 현재 프로세스 동작 경로를 찾는다.
	BOOL hasViewOptionFile = FALSE;
	
	TCHAR currentDirectory[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, currentDirectory);
	TCHAR pathFormat[] = "%s\\%s";
	TCHAR fileName[] = "ViewOption.dat";
	CString path;
	path.Format(pathFormat, currentDirectory, fileName);
	
	//2. 파일이 존재하면, 파일에서 읽는다.
	char readMode[] = "rb";
	FILE* file = fopen((LPCTSTR)path, readMode);
	if (file != NULL)
	{
		hasViewOptionFile = TRUE;

		fread(&logFont, sizeof(LOGFONT), 1, file);
		fread(&isAutoWrapped, sizeof(BOOL), 1, file);
		fread(&magnification, sizeof(double), 1, file);
		fread(&isStatusBarVisible, sizeof(BOOL), 1, file);

		fclose(file);
	}

	return hasViewOptionFile;
}

void ViewOptionStorage::Save(const LOGFONT& logFont, BOOL isAutoWrapped, double magnification, BOOL isStatusBarVisible) {
	//1. 현재 프로세스 동작 경로를 찾는다.
	TCHAR currentDirectory[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, currentDirectory);
	TCHAR pathFormat[] = "%s\\%s";
	TCHAR fileName[] = "ViewOption.dat";
	CString path;
	path.Format(pathFormat, currentDirectory, fileName);

	//2. 파일에 쓴다.
	char writeMode[] = "wb";
	FILE* file = fopen((LPCTSTR)path, writeMode);
	if (file != NULL)
	{
		fwrite(&logFont, sizeof(LOGFONT), 1, file);
		fwrite(&isAutoWrapped, sizeof(BOOL), 1, file);
		fwrite(&magnification, sizeof(double), 1, file);
		fwrite(&isStatusBarVisible, sizeof(BOOL), 1, file);

		fclose(file);
	}
}