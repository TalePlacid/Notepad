#include "SizeCalculator.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "ByteChecker.h"
#include "TabStopCalculator.h"

#pragma warning(disable:4996)

SizeCalculator::SizeCalculator(CWnd* parent) {
	this->parent = parent;
	
	CDC *cdc = this->parent->GetDC();
	CFont* oldFont = cdc->SelectObject(((NotepadForm*)parent)->GetDisplayFont());
	char character;
	this->singleByteWidths = new Long[95];
	Long maxCharacterWidth = 0;

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
	cdc->GetTextMetrics(&tm);

	this->averageCharacterWidth = tm.tmAveCharWidth;
	this->rowHeight = tm.tmHeight;

	cdc->SelectObject(oldFont);

	this->parent->ReleaseDC(cdc);
}

SizeCalculator::~SizeCalculator() {
	if (this->singleByteWidths != 0)
	{
		delete[] this->singleByteWidths;
	}
}

Long SizeCalculator::GetCharacterWidth(char(*character), Long logicalX) {
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
		width = TabStopCalculator::CalculateMarginToNext(logicalX, this->averageCharacterWidth);
	}

	return width;
}

Long SizeCalculator::GetRowWidth(CString contents) {
	CDC* cdc = this->parent->GetDC();
	CFont* oldFont = cdc->SelectObject(((NotepadForm*)this->parent)->GetDisplayFont());
	Long rowWidth = cdc->GetTextExtent(contents).cx;
	cdc->SelectObject(oldFont);
	this->parent->ReleaseDC(cdc);

	return rowWidth;
}

Long SizeCalculator::GetRowWidth(Glyph* row, Long columnIndex) {
	
	char(*character);
	Long width = 0;
	Long i = 0;
	while (i < columnIndex && i < row->GetLength())
	{
		character = (char*)(*row->GetAt(i));
		if (!ByteChecker::IsASCII(character))
		{
			width += this->multiByteWidth;
		}
		else if (*character != '\t')
		{
			width += this->singleByteWidths[*character - 32];
		}
		else
		{
			width += TabStopCalculator::CalculateMarginToNext(width, this->averageCharacterWidth);
		}
		i++;
	}

	return width;
}

Long SizeCalculator::GetNearestColumnIndex(Glyph* row, Long width) {
	
	char(*character);
	Long previousRowWidth = 0;
	Long rowWidth = 0;
	Long i = 0;
	while (i < row->GetLength() && rowWidth < width)
	{
		previousRowWidth = rowWidth;
		character = (char*)(*row->GetAt(i));
		if (!ByteChecker::IsASCII(character))
		{
			rowWidth += this->multiByteWidth;
		}
		else if (*character != '\t')
		{
			rowWidth += this->singleByteWidths[*character - 32];
		}
		else
		{
			rowWidth += TabStopCalculator::CalculateMarginToNext(rowWidth, this->averageCharacterWidth);
		}
		i++;
	}

	Long index = i;
	if (width - previousRowWidth < rowWidth - width)
	{
		index = i - 1;
	}

	return index;
}
