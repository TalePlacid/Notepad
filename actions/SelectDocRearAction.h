#ifndef _SELECTDOCREARACTION_H //guard
#define _SELECTDOCREARACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectDocRearAction : public Action {
public:
	SelectDocRearAction(CWnd* parent);
	virtual ~SelectDocRearAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectDocRearAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTDOCREARACTION_H
