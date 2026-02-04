#include "NotepadApp.h"
#include "NotepadFrame.h"
#include "resource.h"

#pragma warning(disable:4996)

NotepadApp::NotepadApp() {

}

NotepadApp::~NotepadApp() {

}

BOOL NotepadApp::InitInstance() {
#if 0
	//테스트용 파일경로
	CString testPath = "C:\\parkcom\\notepad\\notepad\\test.txt";
	this->m_lpCmdLine = testPath.GetBuffer();
#endif
	NotepadFrame* notepadFrame = new NotepadFrame(this->m_lpCmdLine);

	notepadFrame->Create(NULL, "메모장 ~제목없음");

	notepadFrame->ShowWindow(SW_SHOW);
	notepadFrame->UpdateWindow();
	this->m_pMainWnd = notepadFrame;

	if (this->m_lpCmdLine[0] != '\0')
	{
		SendMessage(notepadFrame->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_OPENINEXPLORER, 0);
	}

	return TRUE;
}

NotepadApp notepadApp;