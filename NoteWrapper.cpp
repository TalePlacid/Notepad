#include <afxwin.h>
#include "NoteWrapper.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "glyphs/NoteWidthCache.h"
#include "SizeCalculator.h"
#include "ScrollController.h"
#include "glyphs/GlyphFactory.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

NoteWrapper::NoteWrapper(CWnd* parent) {
	this->parent = parent;
}

NoteWrapper::~NoteWrapper() {

}

Long NoteWrapper::Wrap() {
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	ClientAreaSize clientAreaSize = ((NotepadForm*)(this->parent))->GetClientAreaSize();
	Long clientAreaWidth = clientAreaSize.width;
	Long count = 0;
	Long previousWordStart;
	Long rowWidth;
	Long j;
	Long cuttingIndex;
	Long i = 0;
	while (i < note->GetLength())
	{
		cuttingIndex = sizeCalculator->GetWrapCuttingIndex(i);
		if (cuttingIndex > 0)
		{
			row = note->GetAt(i);
			if (cuttingIndex < row->GetLength() && ByteChecker::IsAlphabet((char*)(*row->GetAt(cuttingIndex))))
			{
				previousWordStart = row->FindPreviousWordStart(cuttingIndex);
				if (previousWordStart > 0)
				{
					cuttingIndex = previousWordStart;
				}
			}

			note->SplitRows(i, cuttingIndex, true);
			noteWidthCache->Add(i + 1);
			noteWidthCache->MarkDirty(i);
			noteWidthCache->MarkDirty(i + 1);
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
	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	Glyph* previousRow;
	while (row->IsDummyRow() && rowIndex > 0)
	{
		previousRow = note->GetAt(rowIndex - 1);
		columnIndex += previousRow->GetLength();
		note->MergeRows(rowIndex - 1);
		noteWidthCache->MarkDirty(rowIndex - 1);
		noteWidthCache->Remove(rowIndex);
		count--;

		rowIndex = note->Previous();
		row = note->GetAt(rowIndex);
		columnIndex = row->Move(columnIndex);
	}

	//3. 더미 줄들을 아래로 합친다.
	while (note->GetLength() > rowIndex + 1 && note->GetAt(rowIndex+1)->IsDummyRow())
	{
		note->MergeRows(rowIndex);
		noteWidthCache->MarkDirty(rowIndex);
		noteWidthCache->Remove(rowIndex + 1);
		count--;
	}

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	ClientAreaSize clientAreaSize = ((NotepadForm*)(this->parent))->GetClientAreaSize();
	Long clientAreaWidth = clientAreaSize.width;

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
		cuttingIndex = sizeCalculator->GetWrapCuttingIndex(i);
		if (cuttingIndex > 0)
		{
			flag = TRUE;
			row = note->GetAt(i);
			if (cuttingIndex < row->GetLength() && ByteChecker::IsAlphabet((char*)(*row->GetAt(cuttingIndex))))
			{
				previousWordStart = row->FindPreviousWordStart(cuttingIndex);
				if (previousWordStart > 0)
				{
					cuttingIndex = previousWordStart;
				}
			}

			note->SplitRows(i, cuttingIndex, true);
			noteWidthCache->Add(i + 1);
			noteWidthCache->MarkDirty(i);
			noteWidthCache->MarkDirty(i + 1);
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

Long NoteWrapper::Unwrap() {
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	NoteWidthCache* noteWidthCache = ((NotepadForm*)(this->parent))->noteWidthCache;
	Long count = 0;
	Long i = 1;              
	while (i < rowIndex && i < note->GetLength())
	{
		row = note->GetAt(i);
		if (row->IsDummyRow())
		{
			note->MergeRows(--i);
			noteWidthCache->MarkDirty(i);
			noteWidthCache->Remove(i + 1);
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
			noteWidthCache->MarkDirty(i);
			noteWidthCache->Remove(i + 1);

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
			noteWidthCache->MarkDirty(i);
			noteWidthCache->Remove(i + 1);
			count++;
		}
		i++;
	}

	return count;
}
