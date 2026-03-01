#include <afxwin.h>
#include "GoToForm.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(GoToForm, CDialog)
	ON_WM_CLOSE()
	ON_COMMAND(IDCANCEL, OnClose)
END_MESSAGE_MAP()

GoToForm::GoToForm(CWnd* parent)
	:CDialog(GoToForm::IDD, parent) {
	this->parent = parent;
	this->lineNumber = -1;
}

GoToForm::~GoToForm() {

}

BOOL GoToForm::OnInitDialog() {
	CDialog::OnInitDialog();

	this->SetWindowText("ÁÙ ÀÌµ¿");
	if (this->parent != NULL)
	{
		RECT parentRect;
		RECT dialogRect;
		this->parent->GetWindowRect(&parentRect);
		this->GetWindowRect(&dialogRect);

		Long width = dialogRect.right - dialogRect.left;
		Long height = dialogRect.bottom - dialogRect.top;
		Long x = parentRect.left + ((parentRect.right - parentRect.left - width) / 2);
		Long y = parentRect.top + ((parentRect.bottom - parentRect.top - height) / 2);
		this->SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	return TRUE;
}

void GoToForm::OnOK() {
	CString lineText;
	this->GetDlgItem(IDC_EDIT_GOTO_LINE)->GetWindowText(lineText);

	if (lineText != "")
	{
		Long parsedLine = atol(lineText);
		if (parsedLine > 0)
		{
			this->lineNumber = parsedLine;
		}
	}

	this->EndDialog(IDOK);
}

void GoToForm::OnClose() {
	this->EndDialog(IDCANCEL);
}
