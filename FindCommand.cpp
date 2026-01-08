#include <afxwin.h>
#include <afxdlgs.h>
#include <string.h>
using namespace std;
#include "FindCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "SearchingAlgorithmFactory.h"
#include "SearchingAlgorithm.h"
#include "Comparer.h"
#include "CaseSensitiveComparer.h"
#include "CaseInsensitiveComparer.h"
#include "SearchResultController.h"
#include "SearchResult.h"
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
	//1.전체 파일에서 단어를 찾는다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	string contents((LPCTSTR)(pagingBuffer->GetFullText()));
	string key((LPCTSTR)(this->findReplaceDialog->GetFindString()));

	Comparer* comparer;
	if (this->findReplaceDialog->MatchCase())
	{
		comparer = new CaseSensitiveComparer;
	}
	else
	{
		comparer = new CaseInsensitiveComparer;
	}

	SearchingAlgorithmFactory searchingAlgorithmFactory;
	SearchingAlgorithm* searchingAlgorithm = searchingAlgorithmFactory.Create(key, contents,
		comparer, SearchingAlgorithmFactory::BRUTE_FORCE, this->findReplaceDialog->MatchWholeWord());

	Long(*offsets) = NULL;
	Long count = 0;
	if (searchingAlgorithm != NULL)
	{
		searchingAlgorithm->DoAlgorithm(&offsets, &count);
		delete searchingAlgorithm;
	}

	//2. 검색결과가 있으면,
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long nearestIndex = -1;
	if (count > 0)
	{
		//2.1. 가장 가까운 위치를 찾는다.
		FindReplaceOption oldOption;
		SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
		if (searchResultController != NULL)
		{
			oldOption = FindReplaceOption(searchResultController->GetKey().c_str(), searchResultController->IsMatchWhole(), searchResultController->IsMatchCase(), searchResultController->IsSearchDown());
			delete ((NotepadForm*)(this->parent))->searchResultController;
			((NotepadForm*)(this->parent))->searchResultController = NULL;
		}

		((NotepadForm*)(this->parent))->searchResultController = new SearchResultController(key,
			this->findReplaceDialog->MatchWholeWord(), this->findReplaceDialog->MatchCase(), 
			this->findReplaceDialog->SearchDown(), offsets, count);
		searchResultController = ((NotepadForm*)(this->parent))->searchResultController;

		Glyph* row;
		Long rowIndex;
		if (this->findReplaceDialog->SearchDown())
		{
			nearestIndex = searchResultController->FindNearestIndexBelow(pagingBuffer->GetCurrentOffset());
		}
		else
		{
			FindReplaceOption currentOption(searchResultController->GetKey().c_str(), searchResultController->IsMatchWhole(), searchResultController->IsMatchCase(), searchResultController->IsSearchDown());
			if (currentOption.EqualsExceptSearchDirection(oldOption))
			{
				nearestIndex = searchResultController->FindNearestIndexAbove(pagingBuffer->GetSelectionBeginOffset());
			}
			else
			{
				nearestIndex = searchResultController->FindNearestIndexAbove(pagingBuffer->GetCurrentOffset());
			}
		}

		//2.2. 가장 가까운 위치가 있으면,
		note->Select(false);
		pagingBuffer->UnmarkSelectionBegin();

		nearestIndex = searchResultController->Move(nearestIndex);
		if (nearestIndex > -1)
		{
			//2.2.1. 가장 가까운 위치로 이동한다.
			Long offset = searchResultController->GetAt(nearestIndex).GetOffset();
			CaretNavigator caretNavigator(this->parent);
			caretNavigator.MoveTo(offset);
			caretNavigator.NormalizeColumn(0);

			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			Long columnIndex = row->GetCurrent();

			Glyph* character;
			Long characterLength;
			ByteChecker byteChecker;
			Long i = 0;
			while (i < key.length())
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
	}

	//3. 검색결과가 없으면, 경고문을 출력한다.
	if (count <= 0 || nearestIndex < 0)
	{
		CString message;
		message.Format("\"%s\"을(를) 찾을 수 없습니다.", key.c_str());
		this->parent->MessageBox(message);
	}

	if (offsets != NULL)
	{
		delete[] offsets;
	}
}