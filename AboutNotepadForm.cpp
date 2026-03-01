#include <afxwin.h>
#include "AboutNotepadForm.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(AboutNotepadForm, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

AboutNotepadForm::AboutNotepadForm(CWnd* parent)
	:CDialog(AboutNotepadForm::IDD, parent) {
	this->parent = parent;
}

AboutNotepadForm::~AboutNotepadForm() {

}

BOOL AboutNotepadForm::OnInitDialog() {
	CDialog::OnInitDialog();

	this->SetWindowText("메모장 정보 / About Notepad");

	CWnd* staticControl = this->GetDlgItem(IDC_STATIC_ABOUT_MESSAGE);
	if (staticControl != NULL)
	{
		staticControl->ShowWindow(SW_HIDE);
	}

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

void AboutNotepadForm::OnPaint() {
	CPaintDC dc(this);

	RECT clientRect;
	this->GetClientRect(&clientRect);
	dc.FillSolidRect(&clientRect, RGB(252, 252, 252));
	dc.SetBkMode(TRANSPARENT);

	int dpiY = dc.GetDeviceCaps(LOGPIXELSY);
	Long titleHeight = -MulDiv(16, dpiY, 72);
	Long bodyHeight = -MulDiv(13, dpiY, 72);

	CFont titleFont;
	CFont bodyFont;
	titleFont.CreateFont(titleHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "맑은 고딕");
	bodyFont.CreateFont(bodyHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "맑은 고딕");

	RECT titleRect = clientRect;
	titleRect.left = 14;
	titleRect.top = 14;
	titleRect.right -= 14;
	titleRect.bottom = titleRect.top + 30;

	RECT bodyRect = clientRect;
	bodyRect.left = 14;
	bodyRect.top = titleRect.bottom + 8;
	bodyRect.right -= 14;
	bodyRect.bottom -= 28;

	CString title = "윈도우즈 메모장 (Windows Notepad)";
	CString body =
		"버전 : 1.00\r\n"
		"제작자 : 임재연\r\n"
		"문의 및 오류 제보 : nastaler@naver.com\r\n"
		"깃허브 : git@github.com:TalePlacid / Notepad.git\r\n"
		"\r\n"
		"Version : 1.00\r\n"
		"Developer : Lim JaeYeon\r\n"
		"Contact / Bug Report : nastaler@naver.com\r\n"
		"Github : git@github.com:TalePlacid / Notepad.git";

	CFont* oldFont = dc.SelectObject(&titleFont);
	dc.SetTextColor(RGB(24, 24, 24));
	dc.DrawText(title, &titleRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	CPen splitPen(PS_SOLID, 1, RGB(220, 220, 220));
	CPen* oldPen = dc.SelectObject(&splitPen);
	dc.MoveTo(14, titleRect.bottom + 2);
	dc.LineTo(clientRect.right - 14, titleRect.bottom + 2);
	dc.SelectObject(oldPen);

	dc.SelectObject(&bodyFont);
	dc.SetTextColor(RGB(50, 50, 50));
	dc.DrawText(body, &bodyRect, DT_LEFT | DT_WORDBREAK);

	dc.SelectObject(oldFont);
}

