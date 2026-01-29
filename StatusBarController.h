#ifndef _STATUSBARCONTROLLER_H //guard
#define _STATUSBARCONTROLLER_H
#include <afxwin.h>
#include "Observer.h"

class CStatusBar;

class StatusBarController : public Observer {
public:
	StatusBarController(CWnd* parent);
	~StatusBarController();

	void Create();
	virtual void Update(Subject* subject, string interest);
private:
	CWnd* parent;
	UINT statuses[5];
	CStatusBar* statusBar;
};


#endif // !_STATUSBARCONTROLLER_H
