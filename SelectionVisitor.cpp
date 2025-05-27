#include <afxwin.h>
#include "SelectionVisitor.h"
#include "Glyph.h"

#pragma warning(disable:4996)

SelectionVisitor::SelectionVisitor(CWnd* parent, CDC* dc, COLORREF highlightBkColor, COLORREF highlightTextColor)
	:Visitor(parent) {
	this->dc = dc;
	this->isHighlighted = false;
	this->defaultBkColor = dc->GetBkColor();
	this->defaultTextColor = dc->GetTextColor();
	this->highlightBkColor = highlightBkColor;
	this->highlightTextColor = highlightTextColor;
}

SelectionVisitor::~SelectionVisitor() {

}

void SelectionVisitor::VisitCharacter(Glyph* character) {
	if (character->IsSelected() && !this->isHighlighted)
	{
		this->dc->SetBkColor(this->highlightBkColor);
		this->dc->SetTextColor(this->highlightTextColor);
		this->isHighlighted = TRUE;
	}
	else if (!character->IsSelected() && this->isHighlighted)
	{
		this->dc->SetBkColor(this->defaultBkColor);
		this->dc->SetTextColor(this->defaultTextColor);
		this->isHighlighted = FALSE;
	}
}