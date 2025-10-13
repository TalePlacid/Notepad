#ifndef _SCROLLBARNECESSITYCHECKER_H //guard
#define _SCROLLBARNECESSITYCHECKER_H
#include <afxwin.h>

typedef signed long int Long;

class ScrollBarNecessityChecker {
public:
	ScrollBarNecessityChecker();
	~ScrollBarNecessityChecker();

	void Check(Long contentsWidth, Long contentsHeight, RECT clientAreaSize, Long scrollBarThickness,
		bool& vScrollNeeded, bool& hScrollNeeded);
};

#endif // !_SCROLLBARNECESSITYCHECKER_H
