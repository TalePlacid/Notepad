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
protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
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

#endif // !_NOTEPADFRAME_H
