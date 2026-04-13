#ifndef _CHANGEINPROGRESSCAPTION_H //guard
#define _CHANGEINPROGRESSCAPTION_H

class CWnd;

class ChangeInProgressCaption {
public:
	ChangeInProgressCaption(CWnd* parent);
	~ChangeInProgressCaption();
private:
	CWnd* parent;
	CString caption;
};

#endif // !_CHANGEINPROGRESSCAPTION_H
