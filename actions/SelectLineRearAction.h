#ifndef _SELECTLINEREARACTION_H //guard
#define _SELECTLINEREARACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class SelectLineRearAction : public Action {
public:
	SelectLineRearAction(CWnd* parent);
	virtual ~SelectLineRearAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool SelectLineRearAction::ShouldKeepSelection() {
	return true;
}

#endif // !_SELECTLINEREARACTION_H
