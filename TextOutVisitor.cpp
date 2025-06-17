#include "TextOutVisitor.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Font.h"
#include "Glyph.h"
#include "SelectionVisitor.h"

#pragma warning(disable:4996)

TextOutVisitor::TextOutVisitor(CWnd* parent, CDC* dc)
	:Visitor(parent){
	this->dc = dc;
	this->selectionVisitor = new SelectionVisitor(parent, dc);
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
	if (this->selectionVisitor != NULL)
	{
		delete this->selectionVisitor;
	}
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

	this->selectionVisitor->VisitCharacter(character);
	
	dc->TextOut(this->x, this->y, CString(character->MakeString().c_str()));

	TCHAR(*content) = (char*)(*character);
	this->x += notepadForm->sizeCalculator->GetCharacterWidth(const_cast<char*>(content));

	if (notepadForm->font != NULL)
	{
		dc->SelectObject(oldFont);
	}
}