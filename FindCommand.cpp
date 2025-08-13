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

#pragma warning(disable:4996)

FindCommand::FindCommand(CWnd* parent, CFindReplaceDialog* findingForm)
	:Command(parent) {
	this->findingForm = findingForm;
}

FindCommand::~FindCommand() {

}

void FindCommand::Execute() {
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

	Long offset = searchResultController->GetAt(0).GetOffset();
	if (pagingBuffer->IsOnPage(offset))
	{
		if (pagingBuffer->GetCurrentOffset() > offset)
		{
			while (pagingBuffer->GetCurrentOffset() > offset)
			{
				pagingBuffer->PreviousRow();
			}

			Long previous = -1;
			while (pagingBuffer->GetCurrentOffset() < offset && previous != pagingBuffer->GetCurrentOffset())
			{
				previous = pagingBuffer->GetCurrentOffset();
				pagingBuffer->Next();
			}

			if (pagingBuffer->GetCurrentOffset() < offset)
			{
				pagingBuffer->NextRow();
				while (pagingBuffer->GetCurrentOffset() < offset)
				{
					previous = pagingBuffer->GetCurrentOffset();
					pagingBuffer->Next();
				}
			}


		}
	}
	else
	{
		pagingBuffer->Load();
	}
}