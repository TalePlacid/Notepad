#include <afxwin.h>
#include "EraseRangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

EraseRangeCommand::EraseRangeCommand(CWnd* parent)
	:Command(parent) {

}

EraseRangeCommand::~EraseRangeCommand() {

}

void EraseRangeCommand::Execute() {
	//1. ��Ʈ���� ���õ� ������ ã�´�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long startRowIndex = note->FindSelectionStart();
	Glyph* row = note->GetAt(startRowIndex);
	Long startColumnIndex = row->FindSelectionStart();

	Long endRowIndex = note->FindSelectionEnd();
	row = note->GetAt(endRowIndex);
	Long endColumnIndex = row->FindSelectionEnd();

	//2. ���ù����� �����.
	Long i;
	if (startRowIndex < endRowIndex)
	{
		row = note->GetAt(startRowIndex);
		row->TruncateAfter(startColumnIndex);

		while (endRowIndex - 1 > startRowIndex)
		{
			note->Remove(startRowIndex + 1);
			endRowIndex--;
		}

		row = note->GetAt(endRowIndex);
		row->TruncateBefore(endColumnIndex);

		note->MergeRows(startRowIndex);
	}
	else
	{
		row = note->GetAt(startRowIndex);
		while (endColumnIndex - 1 >= startColumnIndex)
		{
			row->Remove(startColumnIndex);
			endColumnIndex--;
		}
	}

	note->Move(startRowIndex);
	row = note->GetAt(startRowIndex);
	row->Move(startColumnIndex);

	//3. ����¡���ۿ��� �����.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	pagingBuffer->Remove(pagingBuffer->GetSelectionBeginOffset());
	pagingBuffer->UnmarkSelectionBegin();

	if (currentOffset > selectionBeginOffset)
	{
		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
		}
	}
	else if (currentOffset < selectionBeginOffset)
	{
		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
		}
	}

	//4. ��ũ�ѹ� ��ġ�� �����Ѵ�.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");

	//5. Ŭ���̾�Ʈ ������ �����Ѵ�.
	this->parent->Invalidate();
}