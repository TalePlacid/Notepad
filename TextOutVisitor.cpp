#include "TextOutVisitor.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Font.h"
#include "Glyph.h"

#pragma warning(disable:4996)

TextOutVisitor::TextOutVisitor(CWnd* parent, CDC* dc) {
	this->parent = parent;
	this->dc = dc;
	this->initialX = 0;
	this->initialY = 0;

	SCROLLINFO scrollInfo = {};
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	if (GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo))
	{
		this->initialX -= scrollInfo.nPos;
	}

	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	if (GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo))
	{
		this->initialY -= scrollInfo.nPos;
	}

	this->x = this->initialX;
	this->y = this->initialY;
}

TextOutVisitor::~TextOutVisitor() {

}

void TextOutVisitor::VisitRow(Glyph* row) {
	this->x = initialX;
	this->y += ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();
}

void TextOutVisitor::VisitCharacter(Glyph* character) {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	CFont* oldFont = NULL;
	if (notepadForm->font != NULL)
	{
		oldFont = dc->SelectObject(notepadForm->font->GetCFont());
	}

	COLORREF defaultBkColor;
	COLORREF defaultTextColor;
	if (character->IsSelected())
	{
		defaultBkColor = dc->SetBkColor(RGB(0, 0, 255));
		defaultTextColor = dc->SetTextColor(RGB(255, 255, 255));
	}

	CString content(character->MakeString().c_str());
	dc->TextOut(this->x, this->y, content);
	this->x += notepadForm->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)content));

	if (notepadForm->font != NULL)
	{
		dc->SelectObject(oldFont);
	}

	if (character->IsSelected())
	{
		dc->SetBkColor(defaultBkColor);
		dc->SetTextColor(defaultTextColor);
	}
}