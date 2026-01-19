#include "PreviewCommand.h"
#include "NotepadForm.h"
#include "PreviewForm.h"
#include "SuspendAutoWrap.h"
#include "resource.h"

#pragma warning(disable:4996)

PreviewCommand::PreviewCommand(CWnd* parent)
	:Command(parent) {

}

PreviewCommand::~PreviewCommand() {

}

void PreviewCommand::Execute() {
	SuspendAutoWrap suspendAutoWrap(this->parent);

	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);

	RECT rect;
	rect.left = 0;
	rect.right = 650;
	rect.top = 0;
	rect.bottom = 850;

	if (((NotepadForm*)(this->parent))->previewForm != NULL)
	{
		delete ((NotepadForm*)(this->parent))->previewForm;
	}
	((NotepadForm*)(this->parent))->previewForm = new PreviewForm;

	PreviewForm* previewForm = ((NotepadForm*)(this->parent))->previewForm;
	previewForm->Create(NULL, CString("미리보기"), WS_OVERLAPPEDWINDOW, rect, this->parent);
	previewForm->ShowWindow(TRUE);
	previewForm->UpdateWindow();
}