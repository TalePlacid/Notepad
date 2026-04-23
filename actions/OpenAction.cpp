#include <afxwin.h>
#include <afxdlgs.h>
#include "OpenAction.h"
#include "../NotepadForm.h"
#include "../CaptionController.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../HistoryBook.h"
#include "../NoteConverter.h"
#include "../NoteWrapper.h"
#include "../TextFileIO.h"
#include "../PageManager.h"

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
		//2.1. 문자열을 적재한다.
		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
		notepadForm->AssignSourcePath(fileDialog.GetPathName());
		TextFileIO textFileIO;
		TCHAR(*str) = NULL;
		Long count;
		Encoding encoding = textFileIO.Load((LPCTSTR)(notepadForm->GetSourcePath()), &str, count);

		//2.2. 페이징 버퍼의 내용을 교체한다.
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		pagingBuffer->Clear();
		pagingBuffer->Add(CString(str));
		pagingBuffer->FirstRow();

		//2.3. 첫 페이지를 적재한다.
		PageManager::LoadFirst(this->parent);

		//2.4. 스크롤바와 히스토리북 설정을 초기화한다.
		notepadForm->scrollController->Initialize();
		notepadForm->undoHistoryBook->Clear();
		notepadForm->redoHistoryBook->Clear();

		//2.5. 캡션을 수정한다.
		CString caption = fileDialog.GetFileName();
		Long extensionIndex = caption.ReverseFind('.');
		if (extensionIndex > 0)
		{
			caption = caption.Left(extensionIndex);
		}
		notepadForm->parent->SetWindowTextA((LPCTSTR)caption);
		notepadForm->captionController->UpdateCaption();
		notepadForm->ApplyEncoding(encoding);
		notepadForm->MarkClean();

		if (str != NULL)
		{
			delete[] str;
		}
	}
}



