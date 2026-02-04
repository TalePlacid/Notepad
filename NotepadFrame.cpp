#include "NotepadFrame.h"
#include "NotepadForm.h"
#include "StatusBarController.h"
#include "resource.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(NotepadFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_EXITSIZEMOVE()
	ON_COMMAND_RANGE(ID_MENU_NEW, ID_MENU_PREVIEW, OnCommandRequested)
	ON_COMMAND_RANGE(ID_COMMAND_ERASE, ID_COMMAND_OPENINEXPLORER, OnCommandRequested)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	END_MESSAGE_MAP()

NotepadFrame::NotepadFrame(LPTSTR m_lpCmdLine){
	this->path = "";
	if (m_lpCmdLine[0] != '\0')
	{
		this->path = CString(m_lpCmdLine);
	}
	this->notepadForm = NULL;
	this->statusBarController = NULL;
}

NotepadFrame::~NotepadFrame() {

}

int NotepadFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	//메뉴
	this->menu.LoadMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
	this->SetMenu(&(this->menu));

	//상태바
	this->statusBarController = new StatusBarController(this);

	//에디터
	this->notepadForm = new NotepadForm(this, this->statusBarController);
	CRect clientArea;
	this->GetClientRect(clientArea);
	this->notepadForm->Create(NULL, NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		clientArea, this, IDC_NOTEPADFORM);

	this->notepadForm->SetFocus();

	return 0;
}

void NotepadFrame::OnSetFocus(CWnd* pOldWnd) {
	this->notepadForm->SetFocus();
}

void NotepadFrame::OnSize(UINT nType, int cx, int cy) {
	CFrameWnd::OnSize(nType, cx, cy);

	if (this->notepadForm->GetSafeHwnd() != NULL)
	{
		this->RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDC_NOTEPADFORM);
		this->notepadForm->SetFocus();
	}
}

void NotepadFrame::OnExitSizeMove() {
	this->notepadForm->SendMessage(WM_EXITSIZEMOVE);
}

void NotepadFrame::OnCommandRequested(UINT nID) {
	this->notepadForm->SendMessage(WM_COMMAND, (WPARAM)nID);
	this->notepadForm->SetFocus();
}

BOOL NotepadFrame::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}


void NotepadFrame::OnClose() {
	this->notepadForm->SendMessage(WM_CLOSE);

	if (this->statusBarController != NULL)
	{
		delete this->statusBarController;
	}

	CFrameWnd::OnClose();
}