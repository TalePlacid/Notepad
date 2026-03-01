#include <afxext.h>
#include "StatusBarController.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "PagingBuffer.h"
#include "Encoding.h"
#include "resource.h"

#pragma warning(disable:4996)

StatusBarController::StatusBarController() {
	this->statusBar = NULL;
}

StatusBarController::~StatusBarController() {
	if (this->statusBar != NULL)
	{
		delete this->statusBar;
	}
}

void StatusBarController::Create(CWnd* parent) {
	this->statuses[0] = IDC_STATUS_SEPARATOR;
	this->statuses[1] = IDC_STATUS_CURRENT;
	this->statuses[2] = IDC_STATUS_MAGNIFICATION;
	this->statuses[3] = IDC_STATUS_NEWLINEPOLICY;
	this->statuses[4] = IDC_STATUS_ENCODING;

	this->statusBar = new CStatusBar;
	this->statusBar->Create(parent);

	this->statusBar->SetIndicators(statuses, sizeof(statuses) / sizeof(statuses[0]));
	this->statusBar->SetPaneStyle(0, SBPS_STRETCH);
	this->statusBar->SetPaneInfo(1, this->statuses[0], SBPS_NORMAL, 150);
	this->statusBar->SetPaneInfo(2, this->statuses[0], SBPS_NORMAL, 50);
	this->statusBar->SetPaneInfo(3, this->statuses[0], SBPS_NORMAL, 150);
	this->statusBar->SetPaneInfo(4, this->statuses[0], SBPS_NORMAL, 150);

	this->statusBar->SetPaneText(3, "Windows(CRLF)");
	this->statusBar->SetPaneText(4, "ANSI");
	this->statusBar->ShowWindow(SW_HIDE);
}

BOOL StatusBarController::IsVisible() const {
	return this->statusBar->IsWindowVisible();
}

BOOL StatusBarController::ToggleVisible() {
	BOOL isVisible = this->statusBar->IsWindowVisible();
	if (isVisible)
	{
		this->statusBar->ShowWindow(SW_HIDE);
	}
	else
	{
		this->statusBar->ShowWindow(SW_SHOW);
	}

	return !isVisible;
}

void StatusBarController::Update(Subject* subject, string interest) {
	if (interest == "UpdateStatusBar")
	{
		NotepadForm* notepadForm = (NotepadForm*)(subject);
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Glyph* note = notepadForm->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();
		Long absoluteRow = rowIndex + pagingBuffer->GetRowStartIndex();
		CString current;
		current.Format("ln %ld, col %ld", absoluteRow + 1, columnIndex + 1);
		this->statusBar->SetPaneText(1, (LPCTSTR)current);

		CString magnification;
		Long percentage = notepadForm->GetMagnification() * 100;
		magnification.Format("%ld%%", percentage);
		this->statusBar->SetPaneText(2, magnification);

		CString encoding;
		switch (notepadForm->GetEncoding())
		{
		case UTF16LE:
			encoding = "UTF-16 LE";
			break;
		case UTF16BE:
			encoding = "UTF-16 BE";
			break;
		case UTF8BOM:
			encoding = "UTF-8 BOM";
			break;
		case UTF8:
			encoding = "UTF-8";
			break;
		case ANSI:
			encoding = "ANSI";
			break;
		default:
			break;
		}
		this->statusBar->SetPaneText(4, encoding);
	}
}
