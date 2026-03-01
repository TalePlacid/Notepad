#include "AutoScroller.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "MouseHandler.h"

#pragma warning(disable:4996)

AutoScroller::AutoScroller(CWnd* parent) {
	this->parent = parent;
}

AutoScroller::~AutoScroller() {

}

BOOL AutoScroller::ScrollIfNeeded(CPoint point) {
	BOOL isAutoScrolled = FALSE;

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Long rowHeight = sizeCalculator->GetRowHeight();

		Scroll vScroll = scrollController->GetVScroll();
		Long pos = vScroll.GetPos();
		Long downBorder = vScroll.GetPage() - rowHeight;
		Long pageMaxPos = pos + vScroll.GetPage();
		if (point.y < rowHeight && vScroll.GetPos() > 0)
		{
			scrollController->Up();
			isAutoScrolled = TRUE;
		}
		else if (point.y > downBorder && pageMaxPos < vScroll.GetMax())
		{
			scrollController->Down();
			isAutoScrolled = TRUE;
		}
	}

	if (scrollController->HasHScroll())
	{
		Long multiByteWidth = sizeCalculator->GetMultiByteWidth();
		Scroll hScroll = scrollController->GetHScroll();
		Long pos = hScroll.GetPos();
		Long rightBorder = hScroll.GetPage() - multiByteWidth;
		Long pageMaxPos = pos + hScroll.GetPage();
		if (point.x < multiByteWidth && pos > 0)
		{
			scrollController->Left(multiByteWidth);
			isAutoScrolled = TRUE;
		}
		else if (point.x > rightBorder && pageMaxPos < hScroll.GetMax())
		{
			scrollController->Right(multiByteWidth);
			isAutoScrolled = TRUE;
		}
	}

	return isAutoScrolled;
}
