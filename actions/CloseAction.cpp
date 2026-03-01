#include <afxwin.h>
#include "CloseAction.h"
#include "../NotepadForm.h"

#pragma warning(disable:4996)

CloseAction::CloseAction(CWnd* parent)
	:Action(parent) {

}

CloseAction::~CloseAction() {

}

void CloseAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	CWnd* notepadFrame = notepadForm->parent;
	notepadFrame->PostMessage(WM_CLOSE, 0, 0);
}
