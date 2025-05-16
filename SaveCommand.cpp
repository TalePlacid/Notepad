#include "SaveCommand.h"
#include "NotepadForm.h"
#include <afxdlgs.h>

#pragma warning(disable:4996)

SaveCommand::SaveCommand(CWnd* parent)
	:Command(parent) {

}

SaveCommand::~SaveCommand() {

}

SaveCommand::SaveCommand(const SaveCommand& source)
	:Command(source.parent) {

}

SaveCommand& SaveCommand::operator=(const SaveCommand& source) {
	Command::operator=(source);

	return *this;
}

void SaveCommand::Execute() {
	CFileDialog cFileDialog(FALSE, "txt", "NoName.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "텍스트 파일 (*.txt)|*.txt|모든 파일 (*.*)|*.*||");
	INT_PTR result = cFileDialog.DoModal();
	if (result == IDOK)
	{
		if (((NotepadForm*)(this->parent))->note != NULL)
		{
			CString path = cFileDialog.GetPathName();

			this->parent->SetWindowTextA(cFileDialog.GetFileName());

			((NotepadForm*)(this->parent))->Save(path);
		}
	}
}

void SaveCommand::Unexecute() {

}