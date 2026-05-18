#include <cstdio>
#include "ViewOptionStorage.h"
#include <shlobj.h>

#pragma warning(disable:4996)

ViewOptionStorage::ViewOptionStorage() {

}

ViewOptionStorage::~ViewOptionStorage() {

}

BOOL ViewOptionStorage::Load(LOGFONT& logFont, BOOL& isAutoWrapped, double& magnification, BOOL& isStatusBarVisible) {
	//1. 앱 데이터 경로를 찾는다.
	BOOL hasViewOptionFile = FALSE;
	
	TCHAR appDataPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);

	CString optionDirectory;
	optionDirectory.Format("%s\\SetupNotepad", appDataPath);

	CString path;
	path.Format("%s\\ViewOption.dat", (LPCTSTR)optionDirectory);

	//2. 파일이 존재하면, 파일에서 읽는다.
	FILE* file = fopen((LPCTSTR)path, "rb");
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
	//1. 앱 데이터 경로를 찾는다.
	TCHAR appDataPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);

	CString optionDirectory;
	optionDirectory.Format("%s\\SetupNotepad", appDataPath);

	CreateDirectory((LPCTSTR)optionDirectory, NULL);
	CString path;
	path.Format("%s\\ViewOption.dat", (LPCTSTR)optionDirectory);
	
	//2. 파일에 쓴다.
	FILE* file = fopen((LPCTSTR)path, "wb");
	if (file != NULL)
	{
		fwrite(&logFont, sizeof(LOGFONT), 1, file);
		fwrite(&isAutoWrapped, sizeof(BOOL), 1, file);
		fwrite(&magnification, sizeof(double), 1, file);
		fwrite(&isStatusBarVisible, sizeof(BOOL), 1, file);

		fclose(file);
	}
}