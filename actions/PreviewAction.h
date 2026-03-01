#ifndef _PREVIEWACTION_H //guard
#define _PREVIEWACTION_H
#include "Action.h"

class CWnd;

class PreviewAction : public Action {
public:
	PreviewAction(CWnd* parent);
	virtual ~PreviewAction();

	virtual void Perform();
};

#endif // !_PREVIEWACTION_H

