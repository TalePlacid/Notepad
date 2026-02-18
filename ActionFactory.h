#ifndef _ACTIONFACTORY_H //guard
#define _ACTIONFACTORY_H
#include <afxwin.h>

class Action;

class ActionFactory {
public:
	ActionFactory();
	~ActionFactory();

	Action* Create(CWnd* parent, UINT nChar);
};

#endif // !_ACTIONFACTORY_H
