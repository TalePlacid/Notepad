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

#pragma warning(disable:4996)

FindCommand::FindCommand(CWnd* parent, CFindReplaceDialog* findingForm)
	:Command(parent) {
	this->findingForm = findingForm;
}

FindCommand::~FindCommand() {

}

void FindCommand::Execute() {
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	string contents((LPCTSTR)(pagingBuffer->GetFullText()));
	string key((LPCTSTR)(this->findingForm->GetFindString()));

	Comparer* comparer;
	if (this->findingForm->MatchCase())
	{
		comparer = new CaseSensitiveComparer;
	}
	else
	{
		comparer = new CaseInsensitiveComparer;
	}

	SearchingAlgorithmFactory searchingAlgorithmFactory;
	SearchingAlgorithm* searchingAlgorithm = searchingAlgorithmFactory.Create(key, contents,
		comparer, SearchingAlgorithmFactory::BRUTE_FORCE, this->findingForm->MatchWholeWord());

	Long(*offsets) = NULL;
	Long count;
	if (searchingAlgorithm != NULL)
	{
		searchingAlgorithm->DoAlgorithm(&offsets, &count);
		delete searchingAlgorithm;
	}

	((NotepadForm*)(this->parent))->searchResultController = new SearchResultController(key, offsets, count);
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;

	if (offsets != NULL)
	{
		delete[] offsets;
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Glyph* row;
	Long rowIndex;
	Long offset = searchResultController->GetAt(0).GetOffset();
	if (pagingBuffer->IsOnPage(offset))
	{
		pagingBuffer->MoveOffset(offset);
		rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
		row = note->GetAt(rowIndex);
		row->Move(pagingBuffer->GetCurrent().GetColumn());
	}
	else
	{
		pagingBuffer->Load();
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
		i++;
	}

	this->parent->Invalidate();
}