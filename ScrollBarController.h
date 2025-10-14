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

	void Create();
	virtual void Update(Subject* subject, string interest);

	BOOL HasVSCrollBar() const;
	BOOL HasHScrollBar() const;
private:
	CWnd* parent;
	BOOL hasVScrollBar;
	BOOL hasHScrollBar;
};

inline BOOL ScrollBarController::HasVSCrollBar() const {
	return this->hasVScrollBar;
}

inline BOOL ScrollBarController::HasHScrollBar() const {
	return this->hasHScrollBar;
}

#endif // !_SCROLLBARCONTROLLER_H
