#include "NotepadApp.h"
#include "NotepadFrame.h"

#pragma warning(disable:4996)

NotepadApp::NotepadApp() {

}

NotepadApp::~NotepadApp() {

}

BOOL NotepadApp::InitInstance() {
	NotepadFrame* notepadFrame = new NotepadFrame;

	notepadFrame->Create(NULL, "메모장 ~제목없음");

	notepadFrame->ShowWindow(SW_SHOW);
	notepadFrame->UpdateWindow();
	this->m_pMainWnd = notepadFrame;

	return TRUE;
}

NotepadApp notepadApp;