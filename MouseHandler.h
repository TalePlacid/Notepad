#ifndef _MOUSEHANDLER_H //guard
#define _MOUSEHANDLER_H
#include <afxwin.h>
#define DELTA_PER_TICK 120

class MouseHandler {
public:
	MouseHandler(CWnd* parent);
	~MouseHandler();

	void DownLeftButton(UINT nFlags, CPoint point);
	BOOL WheelMouse(UINT nFlags, short zDeltz, CPoint point);
private:
	CWnd* parent;
};

#endif // !_MOUSEHANDLER_H
