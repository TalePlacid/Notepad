#include "PreviewForm.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CLOSE()
	END_MESSAGE_MAP()


PreviewForm::PreviewForm(CWnd *parent) {
	this->parent = parent;
}

PreviewForm::~PreviewForm() {

}

int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	return 0;
}

void PreviewForm::OnClose() {
	((NotepadForm*)(this->parent))->previewForm = NULL;

	CFrameWnd::OnClose();
}