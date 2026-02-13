#include <afxwin.h>
#include <afxdlgs.h>
#include "SaveAsCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "TextFileIO.h"
#include "resource.h"

#pragma warning(disable:4996)

SaveAsCommand::SaveAsCommand(CWnd* parent)
	:Command(parent) {

}

SaveAsCommand::~SaveAsCommand() {

}

void SaveAsCommand::Execute() {
	//1. 파일 대화상자를 설정한다.
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

	//2. 파일 대화상자에서 경로를 선택되었으면,
	INT_PTR result = fileDialog.DoModal();
	if (result == IDOK)
	{
		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		CString ansi = pagingBuffer->GetFullText();

		TextFileIO textFileIO;
		Encoding encoding = textFileIO.Save(fileDialog.GetPathName(), (LPCTSTR)ansi, ansi.GetLength());

		notepadForm->parent->SetWindowText(fileDialog.GetFileName());
		notepadForm->encoding = encoding;
		notepadForm->isDirty = FALSE;
	}
	
#if 0
	//1. 파일 대화상자를 설정한다.
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

	//2. 파일 대화상자에서 경로를 선택되었으면,
	INT_PTR result = fileDialog.DoModal(); 
	if (result == IDOK)
	{
		//2.1. 경로를 설정한다.
		((NotepadForm*)(this->parent))->sourcePath = fileDialog.GetPathName();
		((NotepadForm*)(this->parent))->parent->SetWindowTextA(fileDialog.GetFileName());

		//2.2. 인코딩을 적는다.
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

		//2.3. 경로에 저장한다.
		((NotepadForm*)(this->parent))->Save(((NotepadForm*)(this->parent))->sourcePath);
		((NotepadForm*)(this->parent))->isDirty = FALSE;
	}
#endif
}