#include <afxdlgs.h>
#include "OpenCommand.h"
#include "NotepadForm.h"
#include "EncodingDetector.h"
#include "NoteConverter.h"
#include "TextEncoder.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

OpenCommand::OpenCommand(CWnd* parent)
	:Command(parent) {
	
}

OpenCommand::~OpenCommand() {

}

void OpenCommand::Execute() {
	//1. 파일탐색 대화상자를 연다.
	CFileDialog cFileDialog(TRUE, "txt", "NoName.txt",
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"텍스트 파일 (*.txt)|*.txt|모든 파일 (*.*)|*.*||");
	INT_PTR result = cFileDialog.DoModal();

	//2. 열렸다면,
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

		//2. 문자열을 적재한다.
		notepadForm->path = cFileDialog.GetPathName();
		TCHAR(*str) = 0;
		Long count;
		notepadForm->Load(notepadForm->path, &str, count);

		//3. ANSI가 아니면, ANSI로 재인코딩한다.
		EncodingDetector encodingDetector;
		TextEncoder textEncoder;
		TCHAR(*encoded) = NULL;
		Long encodedCount;
		if (encodingDetector.IsUTF16LE(str))
		{
			textEncoder.Utf16LeToAnsi(str, count, &encoded, encodedCount);
			notepadForm->encoding = "UTF-16 LE";
		}
		else if (encodingDetector.IsUTF16BE(str))
		{
			textEncoder.Utf16BeToAnsi(str, count, &encoded, encodedCount);
			notepadForm->encoding = "UTF-16 BE";
		}
		else if (encodingDetector.IsUTF8BOM(str))
		{
			textEncoder.Utf8BomToAnsi(str, count, &encoded, encodedCount);
			notepadForm->encoding = "UTF-8 BOM";
		}
		else if (encodingDetector.IsUTF8(str, count))
		{
			textEncoder.Utf8ToAnsi(str, count, &encoded, encodedCount);
			notepadForm->encoding = "UTF-8";
		}
		else
		{
			encoded = new TCHAR[count + 1];
			encoded[count] = '\0';
			memcpy(encoded, str, count);
			notepadForm->encoding = "ANSI";
		}

		//4. 노트를 생성한다.
		NoteConverter noteConverter;
		notepadForm->note = noteConverter.Convert(string(encoded));

		//5. 페이징 버퍼의 내용을 교체한다.
		pagingBuffer->Add(CString(encoded));
		pagingBuffer->FirstRow();

		//6. 캡션을 수정한다.
		notepadForm->parent->SetWindowTextA(cFileDialog.GetFileName());

		if (str != NULL)
		{
			delete[] str;
		}

		if (encoded != NULL)
		{
			delete[] encoded;
		}
	}
}