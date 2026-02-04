#ifndef _MOUSEHANDLER_H //guard
#define _MOUSEHANDLER_H
#include <afxwin.h>

class MouseHandler {
public:
	MouseHandler(CWnd* parent);
	~MouseHandler();

	void DownLeftButton(UINT nFlags, CPoint point);
private:
	CWnd* parent;
};

#endif // !_MOUSEHANDLER_H
