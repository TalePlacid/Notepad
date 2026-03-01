#include <afxwin.h>
#include <afxdlgs.h>
#include "SaveAsAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../TextFileIO.h"

#pragma warning(disable:4996)

SaveAsAction::SaveAsAction(CWnd* parent)
	:Action(parent) {

}

SaveAsAction::~SaveAsAction() {

}

void SaveAsAction::Perform() {
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

		notepadForm->AssignSourcePath(fileDialog.GetPathName());
		notepadForm->ApplyEncoding(encoding);
		notepadForm->MarkClean();
		CString caption = fileDialog.GetFileName();
		Long extensionIndex = caption.ReverseFind('.');
		if (extensionIndex > 0)
		{
			caption = caption.Left(extensionIndex);
		}
		notepadForm->parent->SetWindowText((LPCTSTR)caption);
	}
}



