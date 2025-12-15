#include <afxwin.h>
#include "HScrollBarLeftClickAction.h"
#include "NotepadForm.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

HScrollBarLeftClickAction::HScrollBarLeftClickAction(CWnd* parent)
	:ScrollBarAction(parent) {

}

HScrollBarLeftClickAction::~HScrollBarLeftClickAction() {

}

void HScrollBarLeftClickAction::Perform() {
	CDC* dc = this->parent->GetDC();
	HDC hdc = dc->GetSafeHdc();
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	Long averageWidth = tm.tmAveCharWidth;
	this->parent->ReleaseDC(dc);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll hScroll = scrollController->GetHScroll();
	Long pos = hScroll.GetPos() - averageWidth;
	if (pos < 0)
	{
		pos = 0;
	}
	scrollController->MoveHScroll(pos);
}