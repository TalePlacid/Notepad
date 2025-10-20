#ifndef _SCROLLCONTROLLER_H //guard
#define _SCROLLCONTROLLER_H
#include <string>
using namespace std;
#include "Scroll.h"
#include "Observer.h"

class CWnd;
class Subject;

class ScrollController : public Observer {
public:
	ScrollController(CWnd* parent);
	virtual ~ScrollController();

	virtual void Update(Subject* subject, string interest);
	
	void ResizeVRange(Long max, Long min = 0);
	Long ResizeVPage(Long page);
	void ResizeHRange(Long max, Long min = 0);
	Long ResizeHPage(Long page);

	Long Up();
	Long Down();
	Long Left();
	Long Right();
	Long PageUp();
	Long PageDown();
	Long MoveVScroll(Long pos);
	Long MoveHScroll(Long pos);

	Scroll& GetVScroll() const;
	Scroll& GetHScroll() const;
private:
	CWnd* parent;
	bool hasVScroll;
	bool hasHScroll;
	Scroll vScroll;
	Scroll hScroll;
};

inline Scroll& ScrollController::GetVScroll() const {
	return const_cast<Scroll&>(this->vScroll);
}

inline Scroll& ScrollController::GetHScroll() const {
	return const_cast<Scroll&>(this->hScroll);
}

#endif // !_SCROLLCONTROLLER_H
