#ifndef _CHANGEINPROGRESSCAPTION_H //guard
#define _CHANGEINPROGRESSCAPTION_H

typedef signed long int Long;

class CWnd;

class ChangeInProgressCaption {
public:
	ChangeInProgressCaption();
	~ChangeInProgressCaption();

	static void AddInProgressCaption(CWnd* parent);
	static void RemoveInProgressCaption(CWnd* parent);
};

#endif // !_CHANGEINPROGRESSCAPTION_H
