#include <afxwin.h>
#include <afxdlgs.h>
#include "FindNextCommand.h"
#include "SearchResultController.h"
#include "NotepadForm.h"
#include "ByteChecker.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "PagingNavigator.h"

#pragma warning(disable:4996)

FindNextCommand::FindNextCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:Command(parent) {
	this->findReplaceDialog = findReplaceDialog;
}

FindNextCommand::~FindNextCommand() {

}

void FindNextCommand::Execute() {
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	Long current = searchResultController->GetCurrent();
	Long previous = current;
	if (searchResultController->IsSearchDown())
	{
		current = searchResultController->Next();
	}
	else
	{
		current = searchResultController->Previous();
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	string key = searchResultController->GetKey();
	if (current != previous)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		note->Select(false);

		pagingBuffer->UnmarkSelectionBegin();

		Glyph* row;
		Long rowIndex;
		Long offset = searchResultController->GetAt(current).GetOffset();

		PagingNavigator pagingNavigator(this->parent);
		pagingNavigator.MoveTo(offset);

		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		ByteChecker byteChecker;
		Long i = 0;
		while (i < key.length())
		{
			character = row->GetAt(columnIndex);
			character->Select(true);

			if (pagingBuffer->GetSelectionBeginOffset() < 0)
			{
				pagingBuffer->MarkSelectionBegin();
			}

			if (byteChecker.IsLeadByte(key[i]))
			{
				i++;
			}

			columnIndex = row->Next();
			pagingBuffer->Next();
			i++;
		}
	}
	else if (!this->findReplaceDialog->ReplaceAll())
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", key.c_str());
		this->parent->MessageBox(message);
	}
}