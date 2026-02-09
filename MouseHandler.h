#ifndef _MOUSEHANDLER_H //guard
#define _MOUSEHANDLER_H
#define MIN_MOVED_PIXEL 4
#include <afxwin.h>

typedef signed int long Long;

enum Direction : Long {
	DIRECTION_NONE,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

class MouseHandler {
public:
	MouseHandler(CWnd* parent);
	~MouseHandler();

	CPoint& UpdateLatestPoint(CPoint point);
	BOOL StartDrag();
	BOOL EndDrag();
	Direction CheckDirection(CPoint point);
	BOOL OnDrag();
private:
	CWnd* parent;
	CPoint latestPoint;
	BOOL onDrag;
};

inline BOOL MouseHandler::OnDrag() {
	return this->onDrag;
}

#endif // !_MOUSEHANDLER_H
