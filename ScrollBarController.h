#ifndef _SCROLLBARCONTROLLER_H //guard
#define _SCROLLBARCONTROLLER_H
#include "Observer.h"

typedef signed long int Long;

class CWnd;
class Subject;

class ScrollBarController : public Observer {
public:
	ScrollBarController(CWnd* parent);
	virtual ~ScrollBarController();

	Long PageDown();

	virtual void Update(Subject* subject, string interest);
private:
	CWnd* parent;
};

#endif // !_SCROLLBARCONTROLLER_H
