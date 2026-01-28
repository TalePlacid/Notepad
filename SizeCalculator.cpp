#include "SizeCalculator.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

SizeCalculator::SizeCalculator(CWnd* parent) {
	this->parent = parent;
	
	CDC *cdc = this->parent->GetDC();
	CFont* oldFont = cdc->SelectObject(((NotepadForm*)parent)->displayFont);
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

	if (((NotepadForm*)parent)->font != NULL)
	{
		cdc->SelectObject(oldFont);
	}

	this->parent->ReleaseDC(cdc);
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
	Long rowWidth = cdc->GetTextExtent(contents).cx;
	ReleaseDC(parent->GetSafeHwnd(), cdc->GetSafeHdc());

	return rowWidth;
}

Long SizeCalculator::GetRowWidth(Glyph* row, Long columnIndex) {
	ByteChecker byteChecker;
	char(*character);
	Long width = 0;
	Long i = 0;
	while (i < columnIndex && i < row->GetLength())
	{
		character = (char*)(*row->GetAt(i));
		if (byteChecker.IsLeadByte(*character))
		{
			width += this->multiByteWidth;
		}
		else if (*character != '\t')
		{
			width += this->singleByteWidths[*character - 32];
		}
		else
		{
			width += this->singleByteWidths[0] * 8;
		}
		i++;
	}

	return width;
}

Long SizeCalculator::GetNearestColumnIndex(Glyph* row, Long width) {
	ByteChecker byteChecker;
	char(*character);
	Long previousRowWidth = 0;
	Long rowWidth = 0;
	Long i = 0;
	while (i < row->GetLength() && rowWidth < width)
	{
		previousRowWidth = rowWidth;
		character = (char*)(*row->GetAt(i));
		if (byteChecker.IsLeadByte(*character))
		{
			rowWidth += this->multiByteWidth;
		}
		else if (*character != '\t')
		{
			rowWidth += this->singleByteWidths[*character - 32];
		}
		else
		{
			rowWidth += this->singleByteWidths[0] * 8;
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