#include <afxext.h>
#include "StatusBarController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "resource.h"

#pragma warning(disable:4996)

StatusBarController::StatusBarController(CWnd* parent) {
	this->parent = parent;
	this->statusBar = NULL;
}

StatusBarController::~StatusBarController() {

}

void StatusBarController::Create() {
	this->statuses[0] = IDC_STATUS_SEPARATOR;
	this->statuses[1] = IDC_STATUS_CURRENT;
	this->statuses[2] = IDC_STATUS_MAGNIFICATION;
	this->statuses[3] = IDC_STATUS_NEWLINEPOLICY;
	this->statuses[4] = IDC_STATUS_ENCODING;

	this->statusBar = new CStatusBar;
	this->statusBar->Create(this->parent);

	this->statusBar->SetIndicators(statuses, sizeof(statuses) / sizeof(statuses[0]));
	this->statusBar->SetPaneStyle(0, SBPS_STRETCH);
	this->statusBar->SetPaneInfo(1, this->statuses[0], SBPS_NORMAL, 150);
	this->statusBar->SetPaneInfo(2, this->statuses[0], SBPS_NORMAL, 50);
	this->statusBar->SetPaneInfo(3, this->statuses[0], SBPS_NORMAL, 150);
	this->statusBar->SetPaneInfo(4, this->statuses[0], SBPS_NORMAL, 150);

	CString magnification;
	Long percentage = ((NotepadForm*)(this->parent))->magnification * 100;
	magnification.Format("%ld%%", percentage);
	this->statusBar->SetPaneText(2, magnification);
	this->statusBar->SetPaneText(3, "Windows(CRLF)");
	this->statusBar->SetPaneText(4, "ANSI");
}

void StatusBarController::Update(Subject* subject, string interest) {
	if (interest == "UpdateStatusBar")
	{
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();
		Long absoluteRow = rowIndex + pagingBuffer->GetRowStartIndex();
		CString current;
		current.Format("ln %ld, col %ld", absoluteRow + 1, columnIndex + 1);
		this->statusBar->SetPaneText(1, (LPCTSTR)current);

		CString magnification;
		Long percentage = ((NotepadForm*)(this->parent))->magnification * 100;
		magnification.Format("%ld%%", percentage);
		this->statusBar->SetPaneText(2, magnification);
	}
}