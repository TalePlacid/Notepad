#include <afxdlgs.h>
#include "SaveCommand.h"
#include "NotepadForm.h"
#include "TextFileIO.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

SaveCommand::SaveCommand(CWnd* parent)
	:Command(parent) {

}

SaveCommand::~SaveCommand() {

}

void SaveCommand::Execute() {
	//1. 원본 파일이 없다면,
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	if (notepadForm->sourcePath == "")
	{
		//1.1. 파일 탐색 대화상자를 연다.
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

		//1.2. 선택했다면, 소스 경로를 설정한다.
		INT_PTR result = fileDialog.DoModal();
		if (result == IDOK)
		{
			notepadForm->sourcePath = fileDialog.GetPathName();
		}
	}

	//2. 원본파일이 있으면,
	CString sourcePath(notepadForm->sourcePath);
	if (sourcePath != "")
	{
		//2.1. 경로에 저장한다.
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		CString ansi = pagingBuffer->GetFullText();
		TextFileIO textFileIO;
		Encoding encoding = textFileIO.Save((LPCTSTR)sourcePath, (LPCTSTR)ansi, ansi.GetLength());

		//2.2. 윈도우의 값들을 수정한다.
		Long index = sourcePath.ReverseFind('\\');
		CString fileName = sourcePath.Right(sourcePath.GetLength() - (index + 1));
		fileName.Left(fileName.GetLength() - 3);
		notepadForm->parent->SetWindowTextA(fileName);
		notepadForm->encoding = encoding;
		notepadForm->isDirty = FALSE;
	}
}