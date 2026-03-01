#include <afxwin.h>
#include "FindPreviousAction.h"
#include "../NotepadForm.h"
#include "../Editor.h"

#pragma warning(disable:4996)

FindPreviousAction::FindPreviousAction(CWnd* parent)
	:Action(parent) {

}

FindPreviousAction::~FindPreviousAction() {

}

void FindPreviousAction::Perform() {
	if (((NotepadForm*)(this->parent))->findReplaceDialog != NULL)
	{
		Editor editor(this->parent);
		editor.FindPrevious();
	}
}
