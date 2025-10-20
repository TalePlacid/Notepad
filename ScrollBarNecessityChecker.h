#ifndef _SCROLLBARNECESSITYCHECKER_H //guard
#define _SCROLLBARNECESSITYCHECKER_H

typedef signed long int Long;

class CWnd;

class ScrollBarNecessityChecker {
public:
	ScrollBarNecessityChecker(CWnd *parent);
	~ScrollBarNecessityChecker();

	void Check(bool& vScrollNeeded, bool& hScrollNeeded);
private:
	CWnd* parent;
};

#endif // !_SCROLLBARNECESSITYCHECKER_H
