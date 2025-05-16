#ifndef _CARETCONTROLLER_H //guard
#define _CARETCONTROLLER_H
#include "Observer.h"
#include <afxwin.h>
#include <string>
using namespace std;

typedef signed long int Long;

class SizeCalculator;
class Caret;
class Subject;

class CaretController : public Observer {
public:
	CaretController(CWnd* parent);
	~CaretController();

	Caret* Create();
	Caret* Destroy();
	virtual void Update(Subject *subject, string interest);

	Caret* GetCaret() const;
private:
	CWnd* parent;
	Caret* caret;
};

inline Caret* CaretController::GetCaret() const {
	return const_cast<Caret*>(this->caret);
}

#endif // !_CARETCONTROLLER_H