#ifndef _SAVEASACTION_H
#define _SAVEASACTION_H
#include "Action.h"

class CWnd;

class SaveAsAction : public Action {
public:
	SaveAsAction(CWnd* parent);
	virtual ~SaveAsAction();

	virtual void Perform();
	virtual bool NeedUpdateCaption();
};

inline bool SaveAsAction::NeedUpdateCaption() {
	return true;
}

#endif // !_SAVEASACTION_H

