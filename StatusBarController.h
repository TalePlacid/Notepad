#ifndef _STATUSBARCONTROLLER_H //guard
#define _STATUSBARCONTROLLER_H
#include <afxwin.h>
#include "Observer.h"

class CStatusBar;
class CWnd;

class StatusBarController : public Observer {
public:
	StatusBarController();
	~StatusBarController();

	void Create(CWnd* parent);
	BOOL ToggleVisible();
	BOOL IsVisible() const;
	virtual void Update(Subject* subject, string interest);
private:
	UINT statuses[5];
	CStatusBar* statusBar;
};


#endif // !_STATUSBARCONTROLLER_H

