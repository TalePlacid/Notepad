#ifndef _SELECTDOCFRONTACTION_H //guard
#define _SELECTDOCFRONTACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectDocFrontAction : public Action {
public:
	SelectDocFrontAction(CWnd* parent);
	virtual ~SelectDocFrontAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectDocFrontAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTDOCFRONTACTION_H
