#ifndef _MOUSEACTIONFACTORY_H //guard
#define _MOUSEACTIONFACTORY_H
#include <afxwin.h>

class MouseAction;

class MouseActionFactory {
public:
	MouseActionFactory(CWnd* parent);
	~MouseActionFactory();

	MouseAction* Create(UINT nID, UINT nFlags, CPoint point, short zDelta);
private:
	CWnd* parent;
};


#endif // !_MOUSEACTIONFACTORY_H

