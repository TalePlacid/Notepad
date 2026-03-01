#ifndef _SELECTIONVISITOR_H //guard
#define _SELECTIONVISITOR_H
#include <windef.h>
#include <wingdi.h>
#include "Visitor.h"

class CWnd;
class CDC;
class Glyph;

class SelectionVisitor : public Visitor {
public:
	SelectionVisitor(CWnd* parent, CDC* dc, COLORREF highlightBkColor = RGB(0, 0, 255),
		COLORREF highlightTextColor = RGB(255, 255, 255));
	virtual ~SelectionVisitor();

	virtual void VisitCharacter(Glyph* character);
private:
	CDC* dc;
	bool isHighlighted;
	COLORREF defaultBkColor;
	COLORREF defaultTextColor;
	COLORREF highlightBkColor;
	COLORREF highlightTextColor;
};

#endif // !_SELECTIONVISITOR_H

