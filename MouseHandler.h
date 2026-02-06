#ifndef _MOUSEHANDLER_H //guard
#define _MOUSEHANDLER_H
#include <afxwin.h>
#define DELTA_PER_TICK 120

class MouseHandler {
public:
	MouseHandler(CWnd* parent);
	~MouseHandler();

	CPoint DownLeftButton(CPoint point);
	void EndDrag(CPoint point);
	BOOL WheelMouse(short zDeltz);
	void CtrlWheelMouse(short zDeltz);
	CPoint& UpdateLatestPoint(CPoint point);

	BOOL IsMovedAboveRow(CPoint point);
	BOOL IsMovedBelowRow(CPoint point);
	BOOL IsMovedLeft(CPoint point);
	BOOL IsMovedRight(CPoint point);

	BOOL OnDrag() const;
private:
	CWnd* parent;
	BOOL onDrag;
	CPoint latestPoint;
};

inline BOOL MouseHandler::OnDrag() const {
	return this->onDrag;
}

#endif // !_MOUSEHANDLER_H
