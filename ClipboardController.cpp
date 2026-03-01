#include <WinSock2.h>
#include <windows.h>
#include <cstring>
#include "ClipboardController.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "PagingBuffer.h"
 
#pragma warning(disable:4996)

ClipboardController::ClipboardController(CWnd* parent)
	:content("") {
	this->parent = parent;
}

ClipboardController::~ClipboardController() {

}

BOOL ClipboardController::Copy() {
	BOOL isOpened = FALSE;
	
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetSelectionBeginOffset() >= 0)
	{
		this->content = pagingBuffer->MakeSelectedString();
	}

	if (this->content != "")
	{
		Long retryCount = 0;
		isOpened = OpenClipboard(this->parent->GetSafeHwnd());
		while (!isOpened && retryCount <= 10)
		{
			Sleep(10);
			isOpened = OpenClipboard(this->parent->GetSafeHwnd());
			retryCount++;
		}

		if (isOpened)
		{
			EmptyClipboard();
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen((LPCTSTR)(this->content)) + 1);
			LPVOID lpMem = GlobalLock(hMem);
			memcpy(lpMem, (LPCTSTR)(this->content), strlen((LPCTSTR)(this->content)) + 1);
			GlobalUnlock(hMem);
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
		}
	}

	return isOpened;
}

BOOL ClipboardController::Paste() {
	BOOL isOpened = OpenClipboard(NULL);
	if (isOpened)
	{
		HANDLE hMem = GetClipboardData(CF_TEXT);
		LPVOID lpMem = GlobalLock((HGLOBAL)hMem);
		this->content = CString((LPCTSTR)lpMem);
		GlobalUnlock((HGLOBAL)hMem);
		CloseClipboard();
	}

	return isOpened;
}
