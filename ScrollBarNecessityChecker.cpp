#include "ScrollBarNecessityChecker.h"

#pragma warning(disable:4996)

ScrollBarNecessityChecker::ScrollBarNecessityChecker() {

}

ScrollBarNecessityChecker::~ScrollBarNecessityChecker() {

}

void ScrollBarNecessityChecker::Check(RECT contentsSize, RECT clientAreaSize, 
	Long scrollBarThickness, bool& vScrollNeeded, bool& hScrollNeeded) {
	Long contentsWidth = contentsSize.right - contentsSize.left;
	Long contentsHeight = contentsSize.bottom - contentsSize.top;
	Long clientAreaWidth = clientAreaSize.right - clientAreaSize.left;
	Long clientAreaHeight = clientAreaSize.bottom - clientAreaSize.top;

	vScrollNeeded = contentsHeight > clientAreaHeight;
	hScrollNeeded = contentsWidth > clientAreaWidth;

	for (Long i = 1; i <= 2; i++)
	{
		if (vScrollNeeded)
		{
			Long updatedClientAreaWidth = clientAreaWidth - scrollBarThickness;
			hScrollNeeded = contentsWidth > updatedClientAreaWidth;
		}

		if (hScrollNeeded)
		{
			Long updatedClientAreaHeight = clientAreaHeight - scrollBarThickness;
			vScrollNeeded = contentsHeight > updatedClientAreaHeight;
		}
	}
}