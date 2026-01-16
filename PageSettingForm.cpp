#include "PageSettingForm.h"
#include "PreviewLayout.h"
#include "NotepadForm.h"
#include "PageSetting.h"

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
	this->previewLayout = NULL;
	this->isInitialized = FALSE;
}

PageSettingForm::~PageSettingForm() {
	if (this->previewLayout != NULL)
	{
		delete this->previewLayout;
	}
}

BOOL PageSettingForm::OnInitDialog() {
	CDialog::OnInitDialog();

	//1. 페이지 설정을 읽는다.
	PageSetting pageSetting = ((NotepadForm*)(this->GetParent()))->pageSetting;

	//1. 컨트롤들에서 기본 값을 설정한다.
	CString sizes[] = { "A4", "A5", "A6", "B4", "B5", "레터", "리걸" };
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

	//2. 미리보기의 위치를 정한다.
	this->previewLayout = new PreviewLayout(this);
	this->previewLayout->Locate();

	this->isInitialized = TRUE;

	return FALSE;
}

void PageSettingForm::OnOptionChanged() {
	if (isInitialized)
	{
		//1. 용지크기와 여백을 읽는다.
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

		//2. 빈 공간이 있다면 기본값으로 채워넣는다.
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

		//2. 미리보기의 위치를 정한다.
		this->previewLayout = new PreviewLayout(this);
		this->previewLayout->Locate();

		this->Invalidate();
	}
}

void PageSettingForm::OnPaint() {
	CPaintDC dc(this);

	//1. 종이 그림자 영역을 그린다.
	CPen solidPen(PS_SOLID, 1, RGB(0, 0, 0));   // 검은색 1픽셀 펜
	CBrush blackBrush(RGB(0, 0, 0));      // 검은색 채움

	CPen* pOldPen = dc.SelectObject(&solidPen);
	CBrush* pOldBrush = dc.SelectObject(&blackBrush);

	RECT paperArea = this->previewLayout->GetPaperArea();
	dc.Rectangle(paperArea.left + 5, paperArea.top + 5, paperArea.right + 5, paperArea.bottom + 5);

	//3. 종이 영역을 그린다.
	CBrush* nullBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(nullBrush); //투명색 채움.

	dc.Rectangle(paperArea.left, paperArea.top, paperArea.right, paperArea.bottom);

	//4. 쓰기 영역을 그린다.
	CPen dotPen(PS_DOT, 1, RGB(255, 0, 0)); // 점선, 적색.
	dc.SelectObject(&dotPen);

	RECT writingArea = this->previewLayout->GetWritingArea();
	dc.Rectangle(writingArea.left, writingArea.top, writingArea.right, writingArea.bottom);

	//5. 원래 펜/브러시를 복원한다.
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void PageSettingForm::OnOK() {
	//1. 용지크기와 여백을 읽는다.
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

	//2. 메모장폼에 적는다.
	PageSetting pageSetting;
	pageSetting.paperName = paperName;
	pageSetting.isVertical = isVertical;
	pageSetting.margin.left = atoi(left);
	pageSetting.margin.right = atoi(right);
	pageSetting.margin.up = atoi(up);
	pageSetting.margin.down = atoi(down);
	pageSetting.header = header;
	pageSetting.footer = footer;
	((NotepadForm*)(this->GetParent()))->pageSetting = pageSetting;

	CDialog::OnOK();
}

void PageSettingForm::OnClose() {
	this->EndDialog(0);

	CDialog::OnClose();
}