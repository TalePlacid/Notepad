#include <afxwin.h>
#include "CopyCommand.h"
#include "NotepadForm.h"
#include "ClipboardController.h"

#pragma warning(disable:4996)

CopyCommand::CopyCommand(CWnd* parent)
	:Command(parent) {

}

CopyCommand::~CopyCommand() {

}

void CopyCommand::Execute() {
	((NotepadForm*)(this->parent))->isDirty = TRUE;

	((NotepadForm*)(this->parent))->clipboardController->Copy();
}