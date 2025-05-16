#include <afxwin.h>
#include "CtrlHomeAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

CtrlHomeAction::CtrlHomeAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlHomeAction::~CtrlHomeAction() {

}

void CtrlHomeAction::Perform() {
	Long rowIndex = ((NotepadForm*)(this->parent))->note->First();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

	row->First();

	this->parent->Invalidate();
}