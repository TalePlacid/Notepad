#include "TextOutVisitor.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "Font.h"
#include "Glyph.h"
#include "SelectionVisitor.h"
#include "PagingBuffer.h"
#include "Position.h"

#pragma warning(disable:4996)

TextOutVisitor::TextOutVisitor(CWnd* parent, CDC* dc)
	:Visitor(parent){
	this->dc = dc;
	this->selectionVisitor = new SelectionVisitor(parent, dc);


	//1. 클라이언트 영역 사이즈를 구한다.
	RECT clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);

	//2. 디스크파일 기준 뷰영역을 구한다.
	RECT absoluteArea = clientArea;
	Long hPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	absoluteArea.left += hPos;
	absoluteArea.right += hPos;
	Long vPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	absoluteArea.top += vPos;
	absoluteArea.bottom += vPos;

	//3. Note기준으로 환산한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long startOffset = pagingBuffer->GetStartOffset();
	Long startRowIndex = pagingBuffer->CountRow(startOffset) - 1;
	if (startRowIndex < 0)
	{
		startRowIndex = 0;
	}
	Long startRowHeight = startRowIndex * sizeCalculator->GetRowHeight();

	this->paintingArea = absoluteArea;
	this->paintingArea.top -= startRowHeight;
	this->paintingArea.bottom -= startRowHeight;

	this->xOffset = 0;
	this->yOffset = 0;
	if (hPos > 0)
	{
		this->paintingArea.left -= sizeCalculator->GetMultiByteWidth();
		this->xOffset = sizeCalculator->GetMultiByteWidth();
	}
	if (vPos > 0)
	{
		this->paintingArea.top -= sizeCalculator->GetRowHeight();
		this->yOffset = sizeCalculator->GetRowHeight();
	}

	this->x = 0;
	this->y = 0;
}

TextOutVisitor::~TextOutVisitor() {
	if (this->selectionVisitor != NULL)
	{
		delete this->selectionVisitor;
	}
}

void TextOutVisitor::VisitRow(Glyph* row) {
	this->x = 0;
	this->y += ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();
}

void TextOutVisitor::VisitCharacter(Glyph* character) {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	CFont* oldFont = NULL;
	if (notepadForm->font != NULL)
	{
		oldFont = dc->SelectObject(notepadForm->font->GetCFont());
	}

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

	if (this->x >= this->paintingArea.left
		&& this->x <= this->paintingArea.right
		&& this->y >= this->paintingArea.top
		&& this->y <= this->paintingArea.bottom)
	{
		this->selectionVisitor->VisitCharacter(character);

		Long painterX = this->x - this->paintingArea.left;
		Long painterY = this->y - this->paintingArea.top;
		dc->TextOut(painterX, painterY, CString(character->MakeString().c_str()));
	}

	this->x += notepadForm->sizeCalculator->GetCharacterWidth((char*)(*character));

	if (notepadForm->font != NULL)
	{
		dc->SelectObject(oldFont);
	}
}