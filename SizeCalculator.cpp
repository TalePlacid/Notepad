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
	if (rowWidthCache->IsDirty())
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		rowWidthCache->Recalculate(note->GetAt(rowIndex));
		rowWidthCache->CleanDirty();
	}
	
	Long rowWidth = rowWidthCache->GetAt(columnIndex);
	
	return rowWidth;
}

Long SizeCalculator::GetNearestColumnIndex(Long rowIndex, Long width) {
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	RowWidthCache* rowWidthCache = noteWidthCache->GetAt(rowIndex);
	Long index = -1;

	if (rowWidthCache->IsDirty())
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		rowWidthCache->Recalculate(note->GetAt(rowIndex));
		rowWidthCache->CleanDirty();
	}

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

	return index;
}

Long SizeCalculator::GetWrapCuttingIndex(Long rowIndex) {
	Long index = -1;
	
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	RowWidthCache* rowWidthCache = noteWidthCache->GetAt(rowIndex);

	if (rowWidthCache->IsDirty())
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		rowWidthCache->Recalculate(note->GetAt(rowIndex));
		rowWidthCache->CleanDirty();
	}

	Long width = ((NotepadForm*)(this->parent))->GetClientAreaSize().width;
	Long low = 0;
	Long high = rowWidthCache->GetLength() - 1;
	Long wrapIndex = -1;
	while (low <= high)
	{
		Long mid = low + (high - low) / 2;
		if (rowWidthCache->GetAt(mid) >= width)
		{
			wrapIndex = mid;
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}

	if (wrapIndex >= 0)
	{
		index = wrapIndex - 1;
		if (index <= 0)
		{
			index = 1;
		}
	}

	return index;
}
