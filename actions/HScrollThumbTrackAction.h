#ifndef _HSCROLLTHUMBTRACKACTION_H //guard
#define _HSCROLLTHUMBTRACKACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class HScrollThumbTrackAction : public Action {
public:
	HScrollThumbTrackAction(CWnd* parent);
	virtual ~HScrollThumbTrackAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
	virtual bool NeedInProgressCaption();
	virtual bool ShouldKeepSelection();
};

inline bool HScrollThumbTrackAction::NeedInProgressCaption() {
	return true;
}

#endif // !_HSCROLLTHUMBTRACKACTION_H
