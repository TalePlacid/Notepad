#ifndef _CLIPBOARDCONTROLLER_H //guard
#define _CLIPBOARDCONTROLLER_H
#include <afxwin.h>

class ClipboardController {
public:
	ClipboardController(CWnd* parent);
	~ClipboardController();

	BOOL Copy();
	BOOL Paste();

	CString& GetContent() const;
private:
	CWnd* parent;
	CString content;
};

inline CString& ClipboardController::GetContent() const {
	return const_cast<CString&>(this->content);
}

#endif // !_CLIPBOARDCONTROLLER_H

