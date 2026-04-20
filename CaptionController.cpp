#include <afxwin.h>
#include "CaptionController.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

CaptionController::CaptionController(CWnd *parent) {
	this->parent = parent;

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	notepadForm->GetParent()->GetWindowTextA(this->caption);
}

CaptionController::~CaptionController() {

}

void CaptionController::UpdateCaption() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	notepadForm->GetParent()->GetWindowTextA(this->caption);
}

void CaptionController::AddInProgressCaption() {
	CString inProgressCaption;
	inProgressCaption.Format("%s <Ã³¸®Áß>", this->caption);

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	notepadForm->GetParent()->SetWindowTextA(inProgressCaption);
}

void CaptionController::RemoveInProgressCaption() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	notepadForm->GetParent()->SetWindowTextA(this->caption);
}

void CaptionController::Update(Subject* subject, string interest) {
	if (interest == "UpdateCaptionUnsaved")
	{
		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
		CString caption(this->caption);

		if (notepadForm->GetSourcePath() == "" || notepadForm->IsDirty())
		{
			caption.Format("*%s", (LPCTSTR)this->caption);
		}

		notepadForm->GetParent()->SetWindowTextA(caption);
	}
}