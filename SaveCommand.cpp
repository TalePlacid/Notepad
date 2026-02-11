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
#if 0
	//1. 파일 탐색 대화상자를 설정한다.
	CFileDialog fileDialog(FALSE, "txt", "NoName.txt", 
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"텍스트 파일 (*.txt)|*.txt|모든 파일 (*.*)|*.*||");
	fileDialog.AddComboBox(IDC_COMBO_ENCODING);
	CString encodings[] = { "UTF-16 LE", "UTF-16 BE", "UTF-8 BOM", "UTF-8", "ANSI" };
	for (Long i = 0; i < sizeof(encodings) / sizeof(encodings[0]); i++)
	{
		fileDialog.AddControlItem(IDC_COMBO_ENCODING, i, encodings[i]);
	}
	fileDialog.SetSelectedControlItem(IDC_COMBO_ENCODING, 4);

	//2. 제목없음 파일이면, 경로를 설정한다.
	CString title;
	((NotepadForm*)(this->parent))->parent->GetWindowTextA(title);
	if (title == "메모장 ~제목없음")
	{
		INT_PTR result = fileDialog.DoModal();
		if (result == IDOK)
		{
			((NotepadForm*)(this->parent))->sourcePath = fileDialog.GetPathName();
			((NotepadForm*)(this->parent))->parent->SetWindowTextA(fileDialog.GetFileName());
		}
	}

	//3. 인코딩을 적는다.
	DWORD selected;
	fileDialog.GetSelectedControlItem(IDC_COMBO_ENCODING, selected);
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

	//4. 경로에 저장한다.
	((NotepadForm*)(this->parent))->Save(((NotepadForm*)(this->parent))->sourcePath);
	((NotepadForm*)(this->parent))->isDirty = FALSE;
#endif
}