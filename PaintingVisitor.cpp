#include "PaintingVisitor.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

PaintingVisitor::PaintingVisitor(CWnd* parent, CDC *dc)
	:Visitor(parent) {
	this->dc = dc;
	this->oldFont = dc->SelectObject(((NotepadForm*)(this->parent))->GetDisplayFont());
	this->dc->SetBkMode(TRANSPARENT); //배경색 투명
	this->dc->SetTextColor(RGB(0, 0, 0)); //글자색 검정

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
}

PaintingVisitor::~PaintingVisitor() {
	this->dc->SelectObject(this->oldFont);
}

void PaintingVisitor::VisitRow(Glyph* row) {
	this->logicalX = 0;
	this->logicalY += ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();
}

void PaintingVisitor::VisitCharacter(Glyph* character) {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	char* character_ = (char*)(*character);
	Long characterWidth = sizeCalculator->GetCharacterWidth(character_, this->logicalX);
	Long rowHeight = sizeCalculator->GetRowHeight();

	if (this->logicalX + characterWidth > this->drawingArea.left
		&& this->logicalX < this->drawingArea.right
		&& this->logicalY + rowHeight >= this->drawingArea.top
		&& this->logicalY < this->drawingArea.bottom) 
	{
		Long drawingX = logicalX - this->drawingArea.left;
		Long drawingY = logicalY - this->drawingArea.top;

		//선택되었으면, 배경 그리기
		if (character->IsSelected())
		{
			RECT characterRect;
			characterRect.left = drawingX;
			characterRect.right = drawingX + characterWidth;
			characterRect.top = drawingY;
			characterRect.bottom = drawingY + rowHeight;
			dc->FillSolidRect(&characterRect, RGB(0, 0, 255));
			this->dc->SetTextColor(RGB(255, 255, 255)); //글자색 흰색
		}

		//탭 키가 아니면, 글자 출력.
		if (character_[0] != '\t')
		{
			dc->TextOut(drawingX, drawingY, CString(character->MakeString().c_str()));
		}

		if (character->IsSelected())
		{
			this->dc->SetTextColor(RGB(0, 0, 0)); //글자색 검정 복구
		}
	}

	this->logicalX += characterWidth;
}