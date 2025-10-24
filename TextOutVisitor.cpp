#include "TextOutVisitor.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Font.h"
#include "Glyph.h"
#include "SelectionVisitor.h"
#include "PagingBuffer.h"
#include "Position.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

TextOutVisitor::TextOutVisitor(CWnd* parent, CDC* dc)
	:Visitor(parent) {
	this->dc = dc;
	this->selectionVisitor = new SelectionVisitor(parent, dc);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Scroll vScroll = scrollController->GetVScroll();
	Scroll hScroll = scrollController->GetHScroll();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long yOffset = pagingBuffer->GetRowStartIndex() * sizeCalculator->GetRowHeight();

	this->drawingArea.top = vScroll.GetPos();
	this->drawingArea.bottom = vScroll.GetPos() + vScroll.GetPage();
	this->drawingArea.left = hScroll.GetPos();
	this->drawingArea.right = hScroll.GetPos() + hScroll.GetPage();

	this->logicalX = 0;
	this->logicalY = yOffset;
	this->drawingX = 0;
	this->drawingY = 0;
}

TextOutVisitor::~TextOutVisitor() {
	if (this->selectionVisitor != NULL)
	{
		delete this->selectionVisitor;
	}
}

void TextOutVisitor::VisitRow(Glyph* row) {
	this->drawingX = 0;
	if (this->logicalY >= this->drawingArea.top
		&& this->logicalY <= this->drawingArea.bottom)
	{
		this->drawingY += ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();
	}

	this->logicalX = 0;
	this->logicalY += ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();
}

void TextOutVisitor::VisitCharacter(Glyph* character) {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	CFont* oldFont = NULL;
	if (notepadForm->font != NULL)
	{
		oldFont = dc->SelectObject(notepadForm->font->GetCFont());
	}

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

	if (this->logicalX >= this->drawingArea.left
		&& this->logicalX <= this->drawingArea.right
		&& this->logicalY >= this->drawingArea.top
		&& this->logicalY <= this->drawingArea.bottom)
	{
		this->selectionVisitor->VisitCharacter(character);

		dc->TextOut(drawingX, drawingY, CString(character->MakeString().c_str()));
		this->drawingX += notepadForm->sizeCalculator->GetCharacterWidth((char*)(*character));
	}

	this->logicalX += notepadForm->sizeCalculator->GetCharacterWidth((char*)(*character));

	if (notepadForm->font != NULL)
	{
		dc->SelectObject(oldFont);
	}
}