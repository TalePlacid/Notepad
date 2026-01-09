#include <afxwin.h>
#include <afxdlgs.h>
#include "FindCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "SearchResultController.h"
#include "Glyph.h"
#include "ByteChecker.h"
#include "FindReplaceOption.h"
#include "CaretNavigator.h"

#pragma warning(disable:4996)

FindCommand::FindCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:Command(parent) {
	this->findReplaceDialog = findReplaceDialog;
}

FindCommand::~FindCommand() {

}

void FindCommand::Execute() {
	//1. 검색옵션을 최신화한다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	FindReplaceOption newOption(this->findReplaceDialog->GetFindString(), this->findReplaceDialog->GetReplaceString(),
		this->findReplaceDialog->MatchWholeWord(), this->findReplaceDialog->MatchCase(), this->findReplaceDialog->SearchDown());
	newOption = searchResultController->ChangeFindReplaceOption(newOption);

	//2. 기존의 검색결과들을 지운다.
	searchResultController->Clear();

	//3. 검색한다.
	searchResultController->Search();

	//4. 검색 결과가 있으면,
	Long nearestIndex = -1;
	if (searchResultController->GetLength() > 0)
	{
		//4.1. 가장 가까운 위치를 찾는다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		if (this->findReplaceDialog->SearchDown())
		{
			nearestIndex = searchResultController->FindNearestIndexBelow(currentOffset);
		}
		else
		{
			nearestIndex = searchResultController->FindNearestIndexAbove(currentOffset);
		}

		//4.2. 선택을 해제한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		note->Select(false);
		pagingBuffer->UnmarkSelectionBegin();

		//4.3. 위치로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(searchResultController->GetAt(nearestIndex));
		caretNavigator.NormalizeColumn(0);

		//4.4. 선택한다.
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Glyph* character;
		Long characterLength;
		ByteChecker byteChecker;
		Long i = 0;
		while (i < newOption.findString.GetLength())
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
	
	//3. 검색결과가 없으면, 경고문을 출력한다.
	if (searchResultController->GetLength() <= 0 || nearestIndex < 0)
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)newOption.findString);
		this->parent->MessageBox(message);
	}
}