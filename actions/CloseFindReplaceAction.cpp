#include <afxwin.h>
#include "CloseFindReplaceAction.h"
#include "../FindReplaceOption.h"
#include "../NotepadForm.h"
#include "../searches/SearchResultController.h"

#pragma warning(disable:4996)

CloseFindReplaceAction::CloseFindReplaceAction(CWnd* parent)
	:Action(parent) {

}

CloseFindReplaceAction::~CloseFindReplaceAction() {

}

void CloseFindReplaceAction::Perform() {
	((NotepadForm*)(this->parent))->findReplaceDialog = NULL;
	((NotepadForm*)(this->parent))->searchResultController->ChangeFindReplaceOption(FindReplaceOption());
}

