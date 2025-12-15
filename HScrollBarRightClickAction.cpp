#include <afxwin.h>
#include "HScrollBarRightClickAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

HScrollBarRightClickAction::HScrollBarRightClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarRightClickAction::~HScrollBarRightClickAction() {

}

void HScrollBarRightClickAction::Perform() {
	CDC* dc = this->parent->GetDC();
	HDC hdc = dc->GetSafeHdc();
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	Long averageWidth = tm.tmAveCharWidth;
	this->parent->ReleaseDC(dc);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll hScroll = scrollController->GetHScroll();
	Long pos = hScroll.GetPos() + averageWidth;
	Long posLimit = hScroll.GetMax() - hScroll.GetPage();
	if (pos > posLimit)
	{
		pos = posLimit;
	}
	scrollController->MoveHScroll(pos);
}