#include <afxwin.h>
#include "NoteWrapper.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "SizeCalculator.h"
#include "ScrollController.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

NoteWrapper::NoteWrapper(CWnd* parent) {
	this->parent = parent;
}

NoteWrapper::~NoteWrapper() {

}

Long NoteWrapper::Wrap() {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ByteChecker byteChecker;

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	CRect clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
	Long clientAreaWidth = clientArea.Width();

	Long count = 0;
	Long previousWordStart;
	Long rowWidth;
	Long j;
	Long cuttingIndex;
	Long i = 0;
	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		rowWidth = 0;
		j = 0;
		while (j < row->GetLength() && rowWidth < clientAreaWidth)
		{
			rowWidth += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(j)));
			j++;
		}

		if (rowWidth >= clientAreaWidth)
		{
			cuttingIndex = j - 2;
			if (cuttingIndex < row->GetLength() && byteChecker.IsAlphabet((char*)(*row->GetAt(cuttingIndex))))
			{
				previousWordStart = row->FindPreviousWordStart(cuttingIndex);
				if (previousWordStart > 0)
				{
					cuttingIndex = previousWordStart;
				}
			}

			note->SplitRows(i, cuttingIndex, true);
			count++;
			if (i < rowIndex) // 현재 줄 위치 이전
			{
				rowIndex = note->Next();
			}
			else if (i == rowIndex && columnIndex > cuttingIndex) // 현재 줄 위치     
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->Move(columnIndex - cuttingIndex);
			}
		}
		i++;
	}

	return count;
}

Long NoteWrapper::Rewrap() {
	Long count = 0;

	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

    //2. 더미 줄들을 위로 합친다.
	Glyph* previousRow;
	while (row->IsDummyRow() && rowIndex > 0)
	{
		previousRow = note->GetAt(rowIndex - 1);
		columnIndex += previousRow->GetLength();
		note->MergeRows(rowIndex - 1);

		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Move(columnIndex);
	}

	//3. 더미 줄들을 아래로 합친다.
	while (note->GetLength() > rowIndex + 1 && note->GetAt(rowIndex+1)->IsDummyRow())
	{
		note->MergeRows(rowIndex);
		count--;
	}

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ByteChecker byteChecker;

	CRect clientArea;
	GetClientRect(this->parent->GetSafeHwnd(), &clientArea);
	Long clientAreaWidth = clientArea.Width();

	//4. 다시 개행한다.
	BOOL flag = TRUE;
	Long previousWordStart;
	Long cuttingIndex;
	Long rowWidth;
	Long j;
	Long i = rowIndex;
	while (i < note->GetLength() && flag)
	{
		flag = FALSE;
		row = note->GetAt(i);
		rowWidth = 0;
		j = 0;
		while (j < row->GetLength() && rowWidth < clientAreaWidth)
		{
			rowWidth += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(j)));
			j++;
		}

		if (rowWidth >= clientAreaWidth)
		{
			flag = TRUE;
			cuttingIndex = j - 2;
			if (cuttingIndex < row->GetLength() && byteChecker.IsAlphabet((char*)(*row->GetAt(cuttingIndex))))
			{
				previousWordStart = row->FindPreviousWordStart(cuttingIndex);
				if (previousWordStart > 0)
				{
					cuttingIndex = previousWordStart;
				}
			}

			note->SplitRows(i, cuttingIndex, true);
			count++;
			if (i == rowIndex && columnIndex > cuttingIndex)
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->Move(columnIndex - cuttingIndex);
			}
		}
		i++;
	}

	return count;
}

Long NoteWrapper::Unwrap() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Long count = 0;
	Long i = 1;              
	while (i < rowIndex && i < note->GetLength())
	{
		row = note->GetAt(i);
		if (row->IsDummyRow())
		{
			note->MergeRows(--i);
			rowIndex = note->Previous();
			count++;
		}
		i++;
	}

	if (i < note->GetLength() && i == rowIndex)
	{
		row = note->GetAt(i);
		if (row->IsDummyRow())
		{
			Long previousLength = note->GetAt(i - 1)->GetLength();
			note->MergeRows(--i);
			rowIndex = note->Previous();
			row = note->GetAt(rowIndex);
			columnIndex = row->Move(previousLength + columnIndex);
			count++;
		} 
	}

	while (i < note->GetLength())
	{
		row = note->GetAt(i);
		if (row->IsDummyRow())
		{
			note->MergeRows(--i);
			count++;
		}
		i++;
	}

	return count;
}