#include <afxwin.h>
#include "AboutNotepadAction.h"
#include "../AboutNotepadForm.h"

#pragma warning(disable:4996)

AboutNotepadAction::AboutNotepadAction(CWnd* parent)
	:Action(parent) {

}

AboutNotepadAction::~AboutNotepadAction() {

}

void AboutNotepadAction::Perform() {
	AboutNotepadForm aboutNotepadForm(this->parent);
	aboutNotepadForm.DoModal();
}

