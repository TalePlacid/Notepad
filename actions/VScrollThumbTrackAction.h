#ifndef _VSCROLLTHUMBTRACKACTION_H //guard
#define _VSCROLLTHUMBTRACKACTION_H
#include "Action.h"

typedef signed long int Long;

class CWnd;

class VScrollThumbTrackAction : public Action {
public:
	VScrollThumbTrackAction(CWnd* parent);
	virtual ~VScrollThumbTrackAction();

	virtual void Perform();
	virtual bool NeedScrollBarUpdate();
};

#endif // !_VSCROLLTHUMBTRACKACTION_H
