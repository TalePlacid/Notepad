#include "DragWheelDownAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

DragWheelDownAction::DragWheelDownAction(CWnd* parent, CPoint point)
	:Action(parent), point(point) {

}

DragWheelDownAction::~DragWheelDownAction() {

}

void DragWheelDownAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (scrollController->HasVScroll())
	{
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Scroll vScroll = scrollController->GetVScroll();
		Long displayY = this->point.y - vScroll.GetPos();
		Long pos = vScroll.GetPos() + rowHeight;
		Long posLimit = vScroll.GetMax() - vScroll.GetPage();
		if (displayY < vScroll.GetPage() - rowHeight && pos <= posLimit)
		{
			scrollController->MoveVScroll(pos);
		}
	}
}