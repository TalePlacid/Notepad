#include "SizeCalculator.h"
#include "NotepadForm.h"
#include "glyphs/NoteWidthCache.h"
#include "glyphs/RowWidthCache.h"
#include "MultiByteWidthCache.h"
#include "glyphs/Glyph.h"
#include "ByteChecker.h"
#include "TabStopCalculator.h"

#pragma warning(disable:4996)

SizeCalculator::SizeCalculator(CWnd* parent) {
	this->parent = parent;
	CDC* cdc = this->parent->GetDC();
	CFont* oldFont = cdc->SelectObject(((NotepadForm*)(this->parent))->GetDisplayFont());

	char character;
	this->singleByteWidths = new Long[95];
	for (Long i = 0; i < 95; i++)
	{
		character = i + 32;
		this->singleByteWidths[i] = cdc->GetTextExtent(CString(character)).cx;
	}

	this->multiByteWidthCache = new MultiByteWidthCache(parent);

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

	if (this->multiByteWidthCache != 0)
	{
		delete this->multiByteWidthCache;
	}
}

Long SizeCalculator::GetCharacterWidth(char(*character), Long logicalX) {
	Long width;
	if (character[0] & 0x80)
	{
		width = this->multiByteWidthCache->GetWidth(character);
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

Long SizeCalculator::GetRowWidth(Long rowIndex, Long columnIndex) {
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	RowWidthCache* rowWidthCache = noteWidthCache->GetAt(rowIndex);
	Long rowWidth = -1;
	if (!rowWidthCache->IsDirty())
	{
		rowWidth = rowWidthCache->GetAt(columnIndex);
	}
 
	return rowWidth;
}

Long SizeCalculator::GetNearestColumnIndex(Long rowIndex, Long width) {
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	RowWidthCache* rowWidthCache = noteWidthCache->GetAt(rowIndex);
	Long index = -1;
	if (!rowWidthCache->IsDirty())
	{
		Long length = rowWidthCache->GetLength();
		if (length > 0)
		{
			Long low = 0;
			Long high = length - 1;

			if (width <= rowWidthCache->GetAt(low))
			{
				index = low;
			}
			else if (width >= rowWidthCache->GetAt(high))
			{
				index = high;
			}
			else
			{
				while (low < high)
				{
					Long mid = low + (high - low) / 2;
					Long midWidth = rowWidthCache->GetAt(mid);

					if (midWidth < width)
					{
						low = mid + 1;
					}
					else
					{
						high = mid;
					}
				}

				Long upperIndex = low;
				Long lowerIndex = upperIndex - 1;
				Long upperWidth = rowWidthCache->GetAt(upperIndex);
				Long lowerWidth = rowWidthCache->GetAt(lowerIndex);
				Long upperDistance = upperWidth - width;
				Long lowerDistance = width - lowerWidth;

				if (upperDistance <= lowerDistance)
				{
					index = upperIndex;
				}
				else
				{
					index = lowerIndex;
				}
			}
		}
#if 0
		Long i = 0;
		Long previous = 0;
		Long current = rowWidthCache->GetAt(0);
		i++;
		while (i < rowWidthCache->GetLength() && current < width)
		{
			previous = current;
			current = rowWidthCache->GetAt(i);
			i++;
		}

		if (abs(current - width) <= abs(width - previous))
		{
			index = i - 1;
		}
		else
		{
			index = i - 2;
		}
#endif
	}

	return index;
}