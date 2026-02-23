#ifndef _PRINTACTION_H //guard
#define _PRINTACTION_H
#include "Action.h"

class CWnd;
class PreviewPaginator;

class PrintAction : public Action {
public:
	PrintAction(CWnd* parent);
	virtual ~PrintAction();

	virtual void Perform();
private:
	PreviewPaginator* previewPaginator;
};

#endif // !_PRINTACTION_H
