#include <afxwin.h>
#include "NoteWrapper.h"
#include "NotepadForm.h"
#include "DummyRow.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "SizeCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

NoteWrapper::NoteWrapper(CWnd* parent) {
	this->parent = parent;
}

NoteWrapper::~NoteWrapper() {

}

void NoteWrapper::InsertDummyRows() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Long columnIndex = note->GetAt(rowIndex)->GetCurrent();

	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaWidth = rect.right - rect.left;

	Long k;
	Glyph* dummyRow;
	Long widthSum;
	Long j;
	Glyph* row;
	GlyphFactory glyphFactory;
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		
		widthSum = 0;
		j = 0;
		while (j < row->GetLength() && widthSum <= clientAreaWidth)
		{
			CString character(row->GetAt(j)->MakeString().c_str());
			widthSum += ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)(character)));
			j++;
		}

		if (j < row->GetLength())
		{
			char character = '\n';
			dummyRow = glyphFactory.Create(&character);
			note->Add(i + 1, dummyRow);

			k = j - 1;
			while (k < row->GetLength())
			{
				dummyRow->Add(row->GetAt(k)->Clone());
				k++;
			}

			k = j - 1;
			while (k < row->GetLength())
			{
				row->Remove(k);
			}

			if (i < rowIndex)
			{
				rowIndex++;
			}
			else if (i == rowIndex)
			{
				if (columnIndex > row->GetLength() - 1)
				{
					rowIndex++;
					columnIndex -= row->GetLength();
				}
			}
		}
		i++;
	}

	rowIndex = note->Move(rowIndex);
	columnIndex = note->GetAt(rowIndex)->Move(columnIndex);
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
