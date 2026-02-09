#ifndef _AUTOSCROLLER_H //guard
#define _AUTOSCROLLER_H
#include <afxwin.h>

class AutoScroller {
public:
	AutoScroller(CWnd* parent);
	~AutoScroller();

	BOOL ScrollIfNeeded(CPoint point);
public:
	CWnd* parent;
};

#endif // !_AUTOSCROLLER_H
