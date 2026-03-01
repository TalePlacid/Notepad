#ifndef _SCROLLBARACTIONFACTORY_H //guard
#define _SCROLLBARACTIONFACTORY_H
#include <afxwin.h>

class ScrollBarAction;

class ScrollBarActionFactory {
public:
	ScrollBarActionFactory();
	~ScrollBarActionFactory();

	ScrollBarAction* Create(CWnd *parent, int nBar, UINT nSBCode, UINT nPos);
};

#endif // !_SCROLLBARACTIONFACTORY_H

