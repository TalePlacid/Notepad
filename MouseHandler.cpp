#include "MouseHandler.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "VScrollBarUpClickAction.h"
#include "vScrollBarDownClickAction.h"
#include "CoordinateConverter.h"
#include "resource.h"

#pragma warning(disable:4996)

MouseHandler::MouseHandler(CWnd* parent)
	:latestPoint(-1, -1) {
	this->parent = parent;
	this->onDrag = FALSE;
}

MouseHandler::~MouseHandler() {

}

CPoint& MouseHandler::UpdateLatestPoint(CPoint point) {
	this->latestPoint = point;

	return this->latestPoint;
}

BOOL MouseHandler::StartDrag() {
	this->onDrag = TRUE;

	return this->onDrag;
}

BOOL MouseHandler::EndDrag() {
	this->latestPoint.SetPoint(-1, -1);
	this->onDrag = FALSE;

	return this->onDrag;
}

Direction MouseHandler::CheckDirection(CPoint point) {
	Direction direction = DIRECTION_NONE;

	//드래그 중이면,
	if (this->onDrag)
	{
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long aboveLinePos = this->latestPoint.y / rowHeight * rowHeight;
		Long belowLinePos = aboveLinePos + rowHeight;
		if (point.y < aboveLinePos)
		{
			direction = DIRECTION_UP;
		}
		else if (point.y >= belowLinePos)
		{
			direction = DIRECTION_DOWN;
		}
		else if (abs(point.x - this->latestPoint.x) > MIN_MOVED_PIXEL)
		{
			if (point.x > this->latestPoint.x)
			{
				direction = DIRECTION_RIGHT;
			}
			else if (point.x < this->latestPoint.x)
			{
				direction = DIRECTION_LEFT;
			}
		}
	}

	return direction;
}