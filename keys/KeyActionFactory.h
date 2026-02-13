#ifndef _KEYACTIONFACTORY_H //guard
#define _KEYACTIONFACTORY_H
#include <afxwin.h>

class KeyAction;

class KeyActionFactory {
public:
	KeyActionFactory();
	~KeyActionFactory();

	KeyAction* Create(CWnd* parent, UINT nChar);
};

#endif // !_STRATEGYFACTORY_H
