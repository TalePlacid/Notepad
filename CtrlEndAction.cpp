#include <afxwin.h>
#include "CtrlEndAction.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

CtrlEndAction::CtrlEndAction(CWnd* parent)
	:KeyAction(parent) {

}

CtrlEndAction::~CtrlEndAction() {

}

void CtrlEndAction::Perform() {
	Long rowIndex = ((NotepadForm*)(this->parent))->note->Last();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

	row->Last();

	this->parent->Invalidate();
}