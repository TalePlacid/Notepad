#include <afxwin.h>
#include <afxdlgs.h>
#include "FindNextCommand.h"
#include "SearchResultController.h"
#include "NotepadForm.h"
#include "ByteChecker.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "CaretNavigator.h"

#pragma warning(disable:4996)

FindNextCommand::FindNextCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:Command(parent) {
	this->findReplaceDialog = findReplaceDialog;
}

FindNextCommand::~FindNextCommand() {

}

void FindNextCommand::Execute() {
	//1. 검색결과 컨트롤러에서 검색방향에 따라 이동한다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	FindReplaceOption findReplaceOption = searchResultController->GetFindReplaceOption();
	Long current = searchResultController->GetCurrent();
	Long previous = current;
	if (findReplaceOption.isSearchDown)
	{
		current = searchResultController->Next();
	}
	else
	{
		current = searchResultController->Previous();
	}

	//2. 선택해제한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 이동했으면,
	if (current != previous)
	{
		//2.1. 위치로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(searchResultController->GetAt(current));
		caretNavigator.NormalizeColumn(0);

		//2.2. 선택한다.
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long characterLength;
		ByteChecker byteChecker;
		Long i = 0;
		while (i < findReplaceOption.findString.GetLength())
		{
			characterLength = 0;
			if (columnIndex < row->GetLength())
			{
				characterLength = 1;
				character = row->GetAt(columnIndex);
				if (byteChecker.IsLeadByte(*(char*)*character))
				{
					characterLength = 2;
				}

				row->GetAt(columnIndex)->Select(true);
				columnIndex = row->Next();

				pagingBuffer->BeginSelectionIfNeeded();
				pagingBuffer->Next();
			}
			else
			{
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
			}

			i += characterLength;
		}
	}
	else if (!this->findReplaceDialog->ReplaceAll()) //3. 이동하지 않았고, 전체 바꾸기 도중도 아니라면,
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)findReplaceOption.findString);
		this->parent->MessageBox(message);
	}
}