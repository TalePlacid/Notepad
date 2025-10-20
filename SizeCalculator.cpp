#include "SizeCalculator.h"
#include "NotepadForm.h"
#include "Font.h"

#pragma warning(disable:4996)

SizeCalculator::SizeCalculator(CWnd* parent) {
	this->parent = parent;
	
	CDC *cdc = this->parent->GetDC();
	CFont* oldFont = NULL;
	char character;
	this->singleByteWidths = new Long[95];
	Long maxCharacterWidth = 0;

	if (((NotepadForm*)parent)->font != NULL)
	{
		oldFont = cdc->SelectObject(((NotepadForm*)parent)->font->GetCFont());
	}

	for (Long i = 0; i < 95; i++)
	{
		character = i + 32;
		this->singleByteWidths[i] = cdc->GetTextExtent(CString(character)).cx;
		if (this->singleByteWidths[i] > maxCharacterWidth)
		{
			maxCharacterWidth = this->singleByteWidths[i];
		}
	}

	this->multiByteWidth = cdc->GetTextExtent(CString("°¡")).cx;
	if (this->multiByteWidth > maxCharacterWidth)
	{
		maxCharacterWidth = this->multiByteWidth;
	}

	this->maxCharacterWidth = maxCharacterWidth;

	TEXTMETRIC tm;
	HDC hdc = cdc->GetSafeHdc();
	GetTextMetrics(hdc, &tm);

	this->rowHeight = tm.tmHeight;

	if (((NotepadForm*)parent)->font != NULL)
	{
		cdc->SelectObject(oldFont);
	}

	ReleaseDC(parent->GetSafeHwnd(), cdc->GetSafeHdc());
}

SizeCalculator::~SizeCalculator() {
	if (this->singleByteWidths != 0)
	{
		delete[] this->singleByteWidths;
	}
}

Long SizeCalculator::GetCharacterWidth(char(*character)) {
	Long width;
	
	if (character[0] & 0x80)
	{
		width = this->multiByteWidth;
	}
	else if (character[0] != '\t')
	{
		width = this->singleByteWidths[character[0] - 32];
	}
	else
	{
		width = this->singleByteWidths[0] * 8;
	}

	return width;
}

Long SizeCalculator::GetRowWidth(CString contents) {
	CDC* cdc = this->parent->GetDC();
	CFont* oldFont = NULL;
	if (((NotepadForm*)parent)->font != NULL)
	{
		oldFont = cdc->SelectObject(((NotepadForm*)parent)->font->GetCFont());
	}

	Long rowWidth = cdc->GetTextExtent(contents).cx;
	
	if (((NotepadForm*)parent)->font != NULL)
	{
		cdc->SelectObject(oldFont);
	}
	ReleaseDC(parent->GetSafeHwnd(), cdc->GetSafeHdc());

	return rowWidth;
}