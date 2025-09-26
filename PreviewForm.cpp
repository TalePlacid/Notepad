#include <cmath>
using namespace std;
#include "PreviewForm.h"
#include "SizeCalculator.h"
#include "PaperCalculator.h"
#include "NotepadForm.h"
#include "Paper.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PreviewForm, CDialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_LEFT, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_RIGHT, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_UP, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_DOWN, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_HEADER, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_FOOTER, OnOptionChanged)
	ON_COMMAND(IDC_RADIO_VERTICAL, OnOptionChanged)
	ON_COMMAND(IDC_RADIO_HORIZONTAL, OnOptionChanged)
	ON_COMMAND(IDC_BUTTON_OK, OnClose)
	ON_COMMAND(IDC_BUTTON_CANCEL, OnClose)
	ON_WM_CLOSE()
	END_MESSAGE_MAP()

PreviewForm::PreviewForm(CWnd* parent)
	:CDialog(PreviewForm::IDD, parent) {

}

PreviewForm::~PreviewForm() {

}

BOOL PreviewForm::OnInitDialog() {
	CDialog::OnInitDialog();

	CString sizes[] = { "A4", "A5", "A6", "B4", "B5", "레터", "리걸"};
	for (Long i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++)
	{
		((CComboBox*)(this->GetDlgItem(IDC_COMBO_SIZE)))->AddString(sizes[i]);
	}
	((CComboBox*)(this->GetDlgItem(IDC_COMBO_SIZE)))->SelectString(0, "A4");

	((CButton*)(this->GetDlgItem(IDC_RADIO_VERTICAL)))->SetCheck(BST_CHECKED);

	this->GetDlgItem(IDC_EDIT_LEFT)->SetWindowText("20");
	this->GetDlgItem(IDC_EDIT_RIGHT)->SetWindowText("20");
	this->GetDlgItem(IDC_EDIT_UP)->SetWindowText("25");
	this->GetDlgItem(IDC_EDIT_DOWN)->SetWindowText("25");

	this->Invalidate();

	return FALSE;
}

void PreviewForm::OnOptionChanged() {
	this->Invalidate();
}

void PreviewForm::OnPaint() {
	CPaintDC dc(this);
	
	//1. 종이 영역을 구한다.
	PaperCalculator paperCalculator(this);
	RECT paperArea = paperCalculator.CalculatePaperArea();
	RECT paperAreaInParent = paperArea;
	ScreenToClient(&paperAreaInParent);

	//2. 종이 그림자 영역을 그린다.
	CPen solidPen(PS_SOLID, 1, RGB(0, 0, 0));   // 검은색 1픽셀 펜
	CBrush blackBrush(RGB(0, 0, 0));      // 검은색 채움
	CPen* pOldPen = dc.SelectObject(&solidPen);
	CBrush* pOldBrush = dc.SelectObject(&blackBrush);
	dc.Rectangle(paperAreaInParent.left + 5, paperAreaInParent.top + 5, paperAreaInParent.right + 5, paperAreaInParent.bottom + 5);

	//3. 종이 영역을 그린다.
	CBrush whiteBrush(RGB(255, 255, 255));
	dc.SelectObject(&whiteBrush); // 흰색 채움
	dc.Rectangle(paperAreaInParent.left, paperAreaInParent.top, paperAreaInParent.right, paperAreaInParent.bottom);

	//4. 쓰기 영역을 구한다.
	RECT writingArea = paperCalculator.CalculateWritingArea(paperArea);
	ScreenToClient(&writingArea);
	CPen dotPen(PS_DOT, 1, RGB(255, 0, 0));
	dc.SelectObject(&dotPen);
	dc.Rectangle(writingArea.left, writingArea.top, writingArea.right, writingArea.bottom);

	//5. 원래 펜/브러시를 복원한다.
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

	//6. 페이지당 줄 수를 구한다.
	Long yDpi = dc.GetDeviceCaps(LOGPIXELSY);
	Long rowHeight = ((NotepadForm*)(this->GetParent()))->sizeCalculator->GetRowHeight();
	Long rowHeightMm = (Long)(ceil((double)rowHeight / yDpi * INCH_TO_MM));

	CString size;
	this->GetDlgItem(IDC_COMBO_SIZE)->GetWindowText(size);
	PaperSize paperSize = Paper::GetPaperSize(size);
	BOOL isVertical = ((CButton*)(this->GetDlgItem(IDC_RADIO_VERTICAL)))->GetCheck();
	BOOL isHorizontal = ((CButton*)(this->GetDlgItem(IDC_RADIO_HORIZONTAL)))->GetCheck();
	Long paperHeight = paperSize.height;
	if (!isVertical && isHorizontal)
	{
		paperHeight = paperSize.width;
	}

	Long rowCount = paperHeight / rowHeightMm;
	
	//7. 폰트 크기를 조절한다.

	//8. 머릿말을 출력한다.
	CString header;
	this->GetDlgItem(IDC_EDIT_HEADER)->GetWindowText(header);
	



}

void PreviewForm::OnClose() {
	this->EndDialog(0);

	CDialog::OnClose();
}

