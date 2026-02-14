#ifndef _SCROLLBARACTION_H //guard
#define _SCROLLBARACTION_H
#include "../Action.h"

class CWnd;

class ScrollBarAction : public Action {
public:
	ScrollBarAction(CWnd* parent);
	virtual ~ScrollBarAction() = 0;
};

#endif // !_SCROLLBARACTION_H
