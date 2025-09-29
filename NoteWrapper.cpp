#include <afxwin.h>
#include "NoteWrapper.h"
#include "NotepadForm.h"
#include "DummyRow.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "SizeCalculator.h"
#include "resource.h"
#include "AutoWrappingPolicy.h"
#include "MultiByteWrappingPolicy.h"
#include "SingleByteWrappingPolicy.h"

#pragma warning(disable:4996)

NoteWrapper::NoteWrapper(CWnd* parent) {
	this->parent = parent;
}

NoteWrapper::~NoteWrapper() {

}

void NoteWrapper::InsertDummyRows(Long width) {
	AutoWrappingPolicy* multiByteWrappingPolicy = new MultiByteWrappingPolicy;
	AutoWrappingPolicy* singleByteWrappingPolicy = new SingleByteWrappingPolicy;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Long columnIndex = note->GetAt(rowIndex)->GetCurrent();

	Long cuttingIndex;
	Long k;
	Glyph* dummyRow;
	Long widthSum;
	Long j;
	Glyph* character = NULL;
	Glyph* row;
	GlyphFactory glyphFactory;
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		
		widthSum = 0;
		j = 0;
		while (j < row->GetLength() && widthSum <= width)
		{
			character = row->GetAt(j);
			widthSum += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth((char*)(*character));
			j++;
		}

		if (j < row->GetLength())
		{
			if (character->IsMultiByteCharacter())
			{
				cuttingIndex = multiByteWrappingPolicy->FindWrappingPoint(row, j - 1);
			}
			else
			{
				cuttingIndex = singleByteWrappingPolicy->FindWrappingPoint(row, j - 1);
			}
			note->SplitRows(i, cuttingIndex, true);
		}
		i++;
	}

	rowIndex = note->Move(rowIndex);
	columnIndex = note->GetAt(rowIndex)->Move(columnIndex);

	if (multiByteWrappingPolicy != 0)
	{
		delete multiByteWrappingPolicy;
	}

	if (singleByteWrappingPolicy != 0)
	{
		delete singleByteWrappingPolicy;
	}
}

void NoteWrapper::DeleteDummyRows() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Long j;
	Glyph* previousRow = NULL;
	Long previousRowLength;
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		if (row->IsDummyRow())
		{
			previousRowLength = previousRow->GetLength();
			j = 0;
			while (j < row->GetLength())
			{
				previousRow->Add(row->GetAt(j)->Clone());
				j++;
			}

			note->Remove(i);

			if (i < rowIndex)
			{
				rowIndex--;
			}
			else if (i == rowIndex)
			{
				rowIndex--;
				columnIndex += previousRowLength;
			}
			i--;
		}
		else
		{
			previousRow = row;
		}
		i++;
	}

	rowIndex = note->Move(rowIndex);
	columnIndex = note->GetAt(rowIndex)->Move(columnIndex);
}
