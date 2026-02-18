#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenAction.h"
#include "../NotepadForm.h"
#include "../NoteConverter.h"
#include "../PagingBuffer.h"
#include "../TextFileIO.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

OpenAction::OpenAction(CWnd* parent)
	:Action(parent) {

}

OpenAction::~OpenAction() {

}

void OpenAction::Perform() {
	//1. 파일탐색 대화상자를 연다.
	CFileDialog fileDialog(TRUE, "txt", "NoName.txt",
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"텍스트 파일 (*.txt)|*.txt|모든 파일 (*.*)|*.*||");
	INT_PTR result = fileDialog.DoModal();

	//2. 선택했다면,
	if (result == IDOK)
	{
		//2.1. 기존 내용을 지운다.
		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
		if (notepadForm->note != NULL)
		{
			delete notepadForm->note;
			notepadForm->note = NULL;
		}

		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		pagingBuffer->Clear();

		//2.2. 문자열을 적재한다.
		notepadForm->sourcePath = fileDialog.GetPathName();
		TextFileIO textFileIO;
		TCHAR(*str) = NULL;
		Long count;
		Encoding encoding = textFileIO.Load((LPCTSTR)(notepadForm->sourcePath), &str, count);

		//2.3. 노트를 생성한다.
		NoteConverter noteConverter;
		notepadForm->note = noteConverter.Convert(string(str));

		//2.4. 페이징 버퍼의 내용을 교체한다.
		pagingBuffer->Add(CString(str));
		pagingBuffer->FirstRow();

		//2.5. 스크롤바 설정을 초기화한다.
		notepadForm->scrollController->Initialize();

		//2.6. 캡션을 수정한다.
		notepadForm->parent->SetWindowTextA(fileDialog.GetFileName());
		notepadForm->encoding = encoding;
		notepadForm->isDirty = FALSE;

		if (str != NULL)
		{
			delete[] str;
		}
	}
}