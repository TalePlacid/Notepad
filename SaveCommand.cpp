#include <afxdlgs.h>
#include "SaveCommand.h"
#include "NotepadForm.h"
#include "resource.h"

#pragma warning(disable:4996)

SaveCommand::SaveCommand(CWnd* parent)
	:Command(parent) {

}

SaveCommand::~SaveCommand() {

}

void SaveCommand::Execute() {
	//1. 제목없음 파일이면, 경로를 설정한다.
	CFileDialog cFileDialog(FALSE, "txt", "NoName.txt", 
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"텍스트 파일 (*.txt)|*.txt|모든 파일 (*.*)|*.*||");
	cFileDialog.AddComboBox(IDC_COMBO_ENCODING);
	CString encodings[] = { "UTF-16 LE", "UTF-16 BE", "UTF-8 BOM", "UTF-8", "ANSI" };
	for (Long i = 0; i < sizeof(encodings) / sizeof(encodings[0]); i++)
	{
		cFileDialog.AddControlItem(IDC_COMBO_ENCODING, i, encodings[i]);
	}
	cFileDialog.SetSelectedControlItem(IDC_COMBO_ENCODING, 4);

	CString title;
	((NotepadForm*)(this->parent))->parent->GetWindowTextA(title);
	if (title == "메모장 ~제목없음")
	{
		INT_PTR result = cFileDialog.DoModal();
		if (result == IDOK)
		{
			if (((NotepadForm*)(this->parent))->note != NULL)
			{
				((NotepadForm*)(this->parent))->path = cFileDialog.GetPathName();

				((NotepadForm*)(this->parent))->parent->SetWindowTextA(cFileDialog.GetFileName());
			}
		}
	}

	//2. 경로에 저장한다.
	DWORD selected;
	cFileDialog.GetSelectedControlItem(IDC_COMBO_ENCODING, selected);
	switch (selected)
	{
	case 0:
		((NotepadForm*)(this->parent))->encoding = "UTF-16 LE";
		break;
	case 1:
		((NotepadForm*)(this->parent))->encoding = "UTF-16 BE";
		break;
	case 2:
		((NotepadForm*)(this->parent))->encoding = "UTF-8 BOM";
		break;
	case 3:
		((NotepadForm*)(this->parent))->encoding = "UTF-8";
		break;
	case 4:
		((NotepadForm*)(this->parent))->encoding = "ANSI";
		break;
	default:
		break;
	}

	((NotepadForm*)(this->parent))->Save(((NotepadForm*)(this->parent))->path);
	((NotepadForm*)(this->parent))->isDirty = FALSE;
}