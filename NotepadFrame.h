#ifndef _NOTEPADFRAME_H //guard
#define _NOTEPADFRAME_H
#include <afxwin.h>

class NotepadForm;
class StatusBarController;

class NotepadFrame : public CFrameWnd {
public:
	NotepadFrame();
	virtual ~NotepadFrame();

	CWnd* GetNotepadForm();
	CMenu* GetMenu();
protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSetFocus(CWnd* pOldWnd);
	void OnSize(UINT nType, int cx, int cy);
	void OnExitSizeMove();
	void OnCommandRequested(UINT nID);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CMenu menu;
	CWnd* notepadForm;
	StatusBarController* statusBarController;
};

inline CWnd* NotepadFrame::GetNotepadForm() {
	return this->notepadForm;
}

inline CMenu* NotepadFrame::GetMenu() {
	return &this->menu;
}

#endif // !_NOTEPADFRAME_H
