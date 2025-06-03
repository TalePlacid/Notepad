#include <WinSock2.h>
#include <windows.h>
#include <cstring>
#include "ClipboardController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "MakeSelectedStringVisitor.h"
 
#pragma warning(disable:4996)

ClipboardController::ClipboardController(CWnd* parent)
	:content("") {
	this->parent = parent;
}

ClipboardController::~ClipboardController() {

}

BOOL ClipboardController::Copy() {
	BOOL isOpened = FALSE;
	
	MakeSelectedStringVisitor makeSelectedStringVisitor(this->parent);
	((NotepadForm*)(this->parent))->note->Accept(makeSelectedStringVisitor);
	this->content = makeSelectedStringVisitor.GetSelectedString();

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
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen((LPCTSTR)(this->content)));
			LPVOID lpMem = GlobalLock(hMem);
			memcpy(lpMem, (LPCTSTR)(this->content), strlen((LPCTSTR)(this->content)));
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