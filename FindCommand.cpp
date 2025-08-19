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
#include "MarkingHelper.h"
#include "ByteChecker.h"
#include "FindReplaceOption.h"

#pragma warning(disable:4996)

FindCommand::FindCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:FindReplaceCommand(parent, findReplaceDialog) {
}

FindCommand::~FindCommand() {

}

void FindCommand::Execute() {
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
	Long count;
	if (searchingAlgorithm != NULL)
	{
		searchingAlgorithm->DoAlgorithm(&offsets, &count);
		delete searchingAlgorithm;
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long nearestIndex = -1;
	if (count > 0)
	{
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

		note->Select(false);
		MarkingHelper markingHelper(this->parent);
		markingHelper.Unmark();

		nearestIndex = searchResultController->Move(nearestIndex);
		if (nearestIndex > -1)
		{
			Long offset = searchResultController->GetAt(nearestIndex).GetOffset();
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
			while (i < key.length())
			{
				character = row->GetAt(columnIndex);
				character->Select(true);

				markingHelper.Mark();

				if (byteChecker.IsLeadByte(key[i]))
				{
					i++;
				}

				columnIndex = row->Next();
				pagingBuffer->Next();
				i++;
			}

			((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
			this->parent->Invalidate();
		}
	}

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