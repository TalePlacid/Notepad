#include "PageSettingForm.h"
#include "PageSettingLayout.h"
#include "PageSetting.h"
#include "../NotepadForm.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PageSettingForm, CDialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, OnOptionChanged)
	ON_CBN_EDITCHANGE(IDC_COMBO_SIZE, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_LEFT, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_RIGHT, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_UP, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_DOWN, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_HEADER, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_FOOTER, OnOptionChanged)
	ON_COMMAND(IDC_RADIO_VERTICAL, OnOptionChanged)
	ON_COMMAND(IDC_RADIO_HORIZONTAL, OnOptionChanged)
	ON_COMMAND(IDC_BUTTON_OK, OnOK)
	ON_COMMAND(IDC_BUTTON_CANCEL, OnClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

PageSettingForm::PageSettingForm(CWnd* parent)
	:CDialog(PageSettingForm::IDD, parent) {
	this->pageSettingLayout = NULL;
	this->parent = parent;
	this->isInitialized = FALSE;
}

PageSettingForm::~PageSettingForm() {
	if (this->pageSettingLayout != NULL)
	{
		delete this->pageSettingLayout;
	}
}

BOOL PageSettingForm::OnInitDialog() {
	CDialog::OnInitDialog();

	//1. ?òÏù¥ÏßÄ ?§Ï†ï???ΩÎäî??
	PageSetting pageSetting = ((NotepadForm*)(this->parent))->GetPageSetting();

	//1. Ïª®Ìä∏Î°§Îì§?êÏÑú Í∏∞Î≥∏ Í∞íÏùÑ ?§Ï†ï?úÎã§.
	CString sizes[] = { "A4", "A5", "A6", "B4", "B5", "?àÌÑ∞", "Î¶¨Í±∏" };
	for (Long i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
	{
		((CComboBox*)(this->GetDlgItem(IDC_COMBO_SIZE)))->AddString(sizes[i]);
	}
	((CComboBox*)(this->GetDlgItem(IDC_COMBO_SIZE)))->SetWindowText(pageSetting.paperName);

	if (pageSetting.isVertical)
	{
		((CButton*)(this->GetDlgItem(IDC_RADIO_VERTICAL)))->SetCheck(BST_CHECKED);
	}
	else
	{
		((CButton*)(this->GetDlgItem(IDC_RADIO_HORIZONTAL)))->SetCheck(BST_CHECKED);
	}

	CString text;
	text.Format("%ld", pageSetting.margin.left);
	this->GetDlgItem(IDC_EDIT_LEFT)->SetWindowText(text);
	text.Format("%ld", pageSetting.margin.right);
	this->GetDlgItem(IDC_EDIT_RIGHT)->SetWindowText(text);
	text.Format("%ld", pageSetting.margin.up);
	this->GetDlgItem(IDC_EDIT_UP)->SetWindowText(text);
	text.Format("%ld", pageSetting.margin.down);
	this->GetDlgItem(IDC_EDIT_DOWN)->SetWindowText(text);

	//2. ÎØ∏Î¶¨Î≥¥Í∏∞???ÑÏπòÎ•??ïÌïú??
	this->pageSettingLayout = new PageSettingLayout(this);
	this->pageSettingLayout->Locate();

	this->isInitialized = TRUE;

	return FALSE;
}

void PageSettingForm::OnOptionChanged() {
	if (isInitialized)
	{
		//1. ?©Ï??¨Í∏∞?Ä ?¨Î∞±???ΩÎäî??
		CString paperName;
		this->GetDlgItem(IDC_COMBO_SIZE)->GetWindowText(paperName);

		CString left;
		CString right;
		CString up;
		CString down;
		this->GetDlgItem(IDC_EDIT_LEFT)->GetWindowText(left);
		this->GetDlgItem(IDC_EDIT_RIGHT)->GetWindowText(right);
		this->GetDlgItem(IDC_EDIT_UP)->GetWindowText(up);
		this->GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(down);

		//2. Îπ?Í≥µÍ∞Ñ???àÎã§Î©?Í∏∞Î≥∏Í∞íÏúºÎ°?Ï±ÑÏõå?£Îäî??
		if (paperName == "")
		{
			((CComboBox*)(this->GetDlgItem(IDC_COMBO_SIZE)))->SelectString(0, "A4");
		}

		if (left == "")
		{
			this->GetDlgItem(IDC_EDIT_LEFT)->SetWindowText("0");
		}

		if (right == "")
		{
			this->GetDlgItem(IDC_EDIT_RIGHT)->SetWindowText("0");
		}

		if (up == "")
		{
			this->GetDlgItem(IDC_EDIT_UP)->SetWindowText("0");
		}

		if (down == "")
		{
			this->GetDlgItem(IDC_EDIT_DOWN)->SetWindowText("0");
		}

		//2. ÎØ∏Î¶¨Î≥¥Í∏∞???ÑÏπòÎ•??ïÌïú??
		this->pageSettingLayout->Locate();

		this->Invalidate();
	}
}

void PageSettingForm::OnPaint() {
	CPaintDC dc(this);

	//1. Ï¢ÖÏù¥ Í∑∏Î¶º???ÅÏó≠??Í∑∏Î¶∞??
	CPen solidPen(PS_SOLID, 1, RGB(0, 0, 0));   // Í≤Ä?Ä??1?ΩÏ? ??
	CBrush blackBrush(RGB(0, 0, 0));      // Í≤Ä?Ä??Ï±ÑÏ?

	CPen* pOldPen = dc.SelectObject(&solidPen);
	CBrush* pOldBrush = dc.SelectObject(&blackBrush);

	RECT paperArea = this->pageSettingLayout->GetPaperArea();
	dc.Rectangle(paperArea.left + 5, paperArea.top + 5, paperArea.right + 5, paperArea.bottom + 5);

	//2. Ï¢ÖÏù¥ ?ÅÏó≠??Í∑∏Î¶∞??
	CBrush whiteBrush(RGB(255, 255, 255));
	dc.SelectObject(&whiteBrush); //?∞ÏÉâ Ï±ÑÏ?.

	dc.Rectangle(paperArea.left, paperArea.top, paperArea.right, paperArea.bottom);

	//3. ?¥Î¶¨???ÅÏó≠???§Ï†ï?úÎã§.
	RECT writingArea = pageSettingLayout->GetWritingArea();
	RECT clipingArea;
	clipingArea.left = writingArea.left;
	clipingArea.right = writingArea.right;
	clipingArea.top = paperArea.top;
	clipingArea.bottom = paperArea.bottom;

	CRgn clipRgn;
	clipRgn.CreateRectRgnIndirect(&clipingArea);
	dc.SelectClipRgn(&clipRgn);

	//3. ?∞Ìä∏Î•??§Ï†ï?úÎã§.
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	CFont* pFont = CFont::FromHandle(hFont);
	LOGFONT logFont;
	pFont->GetLogFont(&logFont);
	logFont.lfHeight = -4;
	CFont smallFont;
	smallFont.CreateFontIndirectA(&logFont);
	CFont* oldFont = dc.SelectObject(&smallFont);

	TEXTMETRIC tm = {};
	dc.GetTextMetrics(&tm);
	Long rowHeight = tm.tmHeight;

	CString header("This is example sentence.");
	Long x = writingArea.left;
	Long y = paperArea.top + (writingArea.top - paperArea.top) / 2;
	dc.TextOut(x, y, header);

	CString contents[4] = { CString("1234567890"), CString("abcdefghijklmnopqrstuvwxyz"), CString("Lorem ipsum collabore irum queda azulenoition rado loco kus roda aint engt deleso"), CString(" ")};
	Long i = 0;
	y = writingArea.top;
	while (y < writingArea.bottom - rowHeight)
	{
		dc.TextOut(x, y, contents[i]);
		i++;
		i %= 3;
		y += rowHeight;
	}

	CString footer("footer maybe place in here.");
	y = writingArea.bottom + (paperArea.bottom - writingArea.bottom) / 2;
	dc.TextOut(x, y, footer);

	//4. ?∞Í∏∞ ?ÅÏó≠??Í∑∏Î¶∞??
	CBrush* nullBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen dotPen(PS_DOT, 1, RGB(255, 0, 0)); // ?êÏÑ†, ?ÅÏÉâ.
	dc.SelectObject(&dotPen);
	dc.SelectObject(nullBrush);

	dc.Rectangle(writingArea.left, writingArea.top, writingArea.right, writingArea.bottom);

	//5. ?êÎûò ??Î∏åÎü¨?úÎ? Î≥µÏõê?úÎã§.
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(oldFont);
}

void PageSettingForm::OnOK() {
	//1. ?©Ï??¨Í∏∞?Ä ?¨Î∞±???ΩÎäî??
	CString paperName;
	this->GetDlgItem(IDC_COMBO_SIZE)->GetWindowText(paperName);

	BOOL isVertical = ((CButton*)(this->GetDlgItem(IDC_RADIO_VERTICAL)))->GetCheck();
	BOOL isHorizontal = ((CButton*)(this->GetDlgItem(IDC_RADIO_HORIZONTAL)))->GetCheck();

	CString left;
	CString right;
	CString up;
	CString down;
	this->GetDlgItem(IDC_EDIT_LEFT)->GetWindowText(left);
	this->GetDlgItem(IDC_EDIT_RIGHT)->GetWindowText(right);
	this->GetDlgItem(IDC_EDIT_UP)->GetWindowText(up);
	this->GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(down);

	CString header;
	CString footer;
	this->GetDlgItem(IDC_EDIT_HEADER)->GetWindowText(header);
	this->GetDlgItem(IDC_EDIT_FOOTER)->GetWindowText(footer);

	//2. Î©îÎ™®?•Ìèº???ÅÎäî??
	PageSetting pageSetting;
	pageSetting.paperName = paperName;
	pageSetting.isVertical = isVertical;
	pageSetting.margin.left = atoi(left);
	pageSetting.margin.right = atoi(right);
	pageSetting.margin.up = atoi(up);
	pageSetting.margin.down = atoi(down);
	pageSetting.header = header;
	pageSetting.footer = footer;
	((NotepadForm*)(this->GetParent()))->ApplyPageSetting(pageSetting);

	CDialog::OnOK();
}

void PageSettingForm::OnClose() {
	this->EndDialog(0);

	CDialog::OnClose();
}

