#include <afxwin.h>
#include "FindNextAction.h"
#include "../FindReplaceOption.h"
#include "../NotepadForm.h"
#include "../SearchResultController.h"
#include "../Editor.h"

#pragma warning(disable:4996)

FindNextAction::FindNextAction(CWnd* parent, FindReplaceOption findReplaceOption)
	:Action(parent) {
	this->findReplaceOption = findReplaceOption;
}

FindNextAction::~FindNextAction() {

}

void FindNextAction::Perform() {
	Editor editor(this->parent);
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	if (this->findReplaceOption != searchResultController->GetFindReplaceOption())
	{
		editor.Find(this->findReplaceOption);
	}
	else
	{
		editor.FindNext();
	}
}
