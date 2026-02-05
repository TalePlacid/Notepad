#ifndef _MOUSEHANDLER_H //guard
#define _MOUSEHANDLER_H
#include <afxwin.h>
#define DELTA_PER_TICK 120

class MouseHandler {
public:
	MouseHandler(CWnd* parent);
	~MouseHandler();

	void DownLeftButton(CPoint point);
	BOOL WheelMouse(short zDeltz);
	void CtrlWheelMouse(short zDeltz);
private:
	CWnd* parent;
};

#endif // !_MOUSEHANDLER_H
