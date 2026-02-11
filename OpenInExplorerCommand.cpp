#include "OpenInExplorerCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "EncodingDetector.h"
#include "TextEncoder.h"
#include "NoteConverter.h"
#include "NotepadFrame.h"

#pragma warning(disable:4996)

OpenInExplorerCommand::OpenInExplorerCommand(CWnd* parent)
	:Command(parent) {
}

OpenInExplorerCommand::~OpenInExplorerCommand() {

}

void OpenInExplorerCommand::Execute() {
#if 0
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
	notepadForm->sourcePath = ((NotepadFrame*)(notepadForm->parent))->GetStartedPath();
	TCHAR(*str) = 0;
	Long count;
	notepadForm->Load(notepadForm->sourcePath, &str, count);

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
	Long index = notepadForm->sourcePath.ReverseFind('\\');
	CString fileName = notepadForm->sourcePath.Right(notepadForm->sourcePath.GetLength() - (index + 1));
	notepadForm->parent->SetWindowTextA((LPCTSTR)fileName);

	if (str != NULL)
	{
		delete[] str;
	}

	if (encoded != NULL)
	{
		delete[] encoded;
	}
#endif
}