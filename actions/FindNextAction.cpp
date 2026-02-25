#include <afxwin.h>
#include "FindNextAction.h"
#include "../FindReplaceOption.h"
#include "../NotepadForm.h"
#include "../SearchResultController.h"
#include "../Editor.h"

#pragma warning(disable:4996)

FindNextAction::FindNextAction(CWnd* parent)
	:Action(parent) {
}

FindNextAction::~FindNextAction() {

}

void FindNextAction::Perform() {
	if (((NotepadForm*)(this->parent))->findReplaceDialog != NULL)
	{
		Editor editor(this->parent);
		editor.FindNext();
	}
}