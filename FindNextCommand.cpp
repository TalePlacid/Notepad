#include <afxwin.h>
#include <afxdlgs.h>
#include "FindNextCommand.h"
#include "SearchResultController.h"
#include "NotepadForm.h"
#include "ByteChecker.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"

#pragma warning(disable:4996)

FindNextCommand::FindNextCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:FindReplaceCommand(parent, findReplaceDialog) {

}

FindNextCommand::~FindNextCommand() {

}

void FindNextCommand::Execute() {
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	
	Long previous = searchResultController->GetCurrent();
	Long current;
	if (this->findReplaceDialog->SearchDown())
	{
		current = searchResultController->Next();
	}
	else
	{
		current = searchResultController->Previous();
	}

	CString key = this->findReplaceDialog->GetFindString();
	if (previous != current)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		note->Select(false);
		MarkingHelper markingHelper(this->parent);
		markingHelper.Unmark();

		Glyph* row;
		Long rowIndex;
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long offset = searchResultController->GetAt(current).GetOffset();

		pagingBuffer->MoveOffset(offset);
		if (pagingBuffer->IsOnPage(offset))
		{
			rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
			row = note->GetAt(rowIndex);
			row->Move(pagingBuffer->GetCurrent().GetColumn());
		}
		else
		{
			pagingBuffer->Load();
			note = ((NotepadForm*)(this->parent))->note;
		}

		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		ByteChecker byteChecker;
		Long i = 0;
		while (i < key.GetLength())
		{
			character = row->GetAt(columnIndex);
			character->Select(true);

			markingHelper.Mark();

			if (byteChecker.IsLeadByte(key[i]))
			{
				i++;
			}

			columnIndex = row->Next();
			i++;
		}

		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		this->parent->Invalidate();
	}
	else
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)key);
		this->parent->MessageBox(message);
	}
}