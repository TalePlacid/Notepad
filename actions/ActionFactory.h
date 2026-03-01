#ifndef _ACTIONFACTORY_H //guard
#define _ACTIONFACTORY_H
#include <afxwin.h>
#include "../AppID.h"

class Action;
class FindReplaceOption;

class ActionFactory {
public:
	ActionFactory();
	~ActionFactory();

	static Action* Create(CWnd* parent, AppID nID, FindReplaceOption* findReplaceOption = NULL);
};

#endif // !_ACTIONFACTORY_H

