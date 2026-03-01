#ifndef _NOTEPADFRAME_H //guard
#define _NOTEPADFRAME_H
#include <afxwin.h>

class NotepadForm;
class StatusBarController;

class NotepadFrame : public CFrameWnd {
public:
	NotepadFrame(LPCTSTR m_lpCmdLine);
	virtual ~NotepadFrame();

	CWnd* GetNotepadForm();
	StatusBarController* GetStatusBarController();
	CMenu* GetMenu();
	CString& GetStartedPath() const;
protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSetFocus(CWnd* pOldWnd);
	void OnSize(UINT nType, int cx, int cy);
	void OnExitSizeMove();
	void OnMenuClicked(UINT nID);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CString startedPath;
	CMenu menu;
	CWnd* notepadForm;
	StatusBarController* statusBarController;
};

inline CString& NotepadFrame::GetStartedPath() const {
	return const_cast<CString&>(this->startedPath);
}

inline CWnd* NotepadFrame::GetNotepadForm() {
	return this->notepadForm;
}

inline StatusBarController* NotepadFrame::GetStatusBarController() {
	return this->statusBarController;
}

inline CMenu* NotepadFrame::GetMenu() {
	return &this->menu;
}

#endif // !_NOTEPADFRAME_H



