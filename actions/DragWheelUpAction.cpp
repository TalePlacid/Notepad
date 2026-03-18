#include "DragWheelUpAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

DragWheelUpAction::DragWheelUpAction(CWnd* parent, CPoint point)
	:Action(parent), point(point) {

}

DragWheelUpAction::~DragWheelUpAction() {

}

void DragWheelUpAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	if (scrollController->HasVScroll())
	{
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Scroll vScroll = scrollController->GetVScroll();
		Long displayY = this->point.y - vScroll.GetPos();
		Long pos = vScroll.GetPos() - rowHeight;
		if (displayY >= rowHeight && pos >= 0)
		{
			scrollController->MoveVScroll(pos);
		}
	}
}