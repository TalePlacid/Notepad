#ifndef _MOUSEEVENTRESOLVER_H //guard
#define _MOUSEEVENTRRSOLVER_H
#include <afxwin.h>
#include "AppID.h"

struct ClientAreaSize;

class MouseEventResolver {
public:
	MouseEventResolver();
	~MouseEventResolver();

	static CPoint NormalizePoint(CPoint point, const ClientAreaSize& clientAreaSize);
	static AppID Resolve(AppID rawID, UINT nFlags, CPoint point, BOOL isDragging, short zDelta = 0);
};

#endif // !_MOUSEEVENTRESOLVER_H
