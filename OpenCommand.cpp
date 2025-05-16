#include "OpenCommand.h"
#include "NotepadForm.h"
#include "NoteConverter.h"
#include <afxdlgs.h>

#pragma warning(disable:4996)

OpenCommand::OpenCommand(CWnd* parent)
	:Command(parent) {
	
}

OpenCommand::~OpenCommand() {

}

OpenCommand::OpenCommand(const OpenCommand& source)
	:Command(source.parent) {

}

OpenCommand& OpenCommand::operator=(const OpenCommand& source) {
	Command::operator=(source);

	return *this;
}

void OpenCommand::Execute() {
	CFileDialog cFileDialog(TRUE);
	INT_PTR result = cFileDialog.DoModal();

	if (result == IDOK)
	{
		CString path = cFileDialog.GetPathName();

		this->parent->SetWindowTextA(cFileDialog.GetFileName());
		
		CString note = ((NotepadForm*)(this->parent))->Load(path);

		if (((NotepadForm*)(this->parent))->note != NULL)
		{
			delete ((NotepadForm*)(this->parent))->note;
			((NotepadForm*)(this->parent))->note = NULL;
		}

		NoteConverter noteConverter;
		((NotepadForm*)(this->parent))->note = noteConverter.ConvertToNote((LPCTSTR)note);

		this->parent->Invalidate();
	}

}

void OpenCommand::Unexecute() {

}