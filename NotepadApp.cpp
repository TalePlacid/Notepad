#include "NotepadApp.h"
#include "NotepadFrame.h"
#include "resource.h"

#pragma warning(disable:4996)

NotepadApp::NotepadApp() {

}

NotepadApp::~NotepadApp() {

}

BOOL NotepadApp::InitInstance() {
	CWinApp::InitInstance();

	//테스트용 파일경로
	CString testPath = "C:\\parkcom\\notepad\\notepad\\textFile\\test.txt";
	NotepadFrame* notepadFrame = new NotepadFrame((LPCTSTR)testPath);
	
	//NotepadFrame* notepadFrame = new NotepadFrame(this->m_lpCmdLine);

	notepadFrame->Create(NULL, "메모장 ~제목없음");         

	notepadFrame->ShowWindow(SW_SHOW);
	notepadFrame->UpdateWindow();
	this->m_pMainWnd = notepadFrame;

	return TRUE;
}

NotepadApp notepadApp;
