#include <afxwin.h>
#include "CopyAction.h"
#include "../NotepadForm.h"
#include "../ClipboardController.h"

#pragma warning(disable:4996)

CopyAction::CopyAction(CWnd* parent)
	:Action(parent) {

}

CopyAction::~CopyAction() {

}

void CopyAction::Perform() {
	((NotepadForm*)(this->parent))->clipboardController->Copy();
}
