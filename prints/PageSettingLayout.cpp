#include "PageSettingLayout.h"
#include "../resource.h"
#include "Paper.h"

#pragma warning(disable:4996)

PageSettingLayout::PageSettingLayout(CWnd* parent) {
	this->parent = parent;
}

PageSettingLayout::~PageSettingLayout() {

}

void PageSettingLayout::Locate() {
    //1. 미리보기 영역을 구한다.
    RECT previewArea;
    CWnd* previewControl = this->parent->GetDlgItem(IDC_GROUP_PREVIEW);
    previewControl->GetWindowRect(&previewArea);

    //2. 용지 크기와 방향을 읽는다.
    CString paper;
    this->parent->GetDlgItem(IDC_COMBO_SIZE)->GetWindowText(paper);
    PaperSize paperSize = Paper::GetPaperSize(paper);
    BOOL isVertical = ((CButton*)(this->parent->GetDlgItem(IDC_RADIO_VERTICAL)))->GetCheck();
    BOOL isHorizontal = ((CButton*)(this->parent->GetDlgItem(IDC_RADIO_HORIZONTAL)))->GetCheck();

    //3. 세로방향이면, 용지사이즈를 수정한다.
    Long marginRatio = 2;
    if (!isVertical && isHorizontal)
    {
        Long temp = paperSize.width;
        paperSize.width = paperSize.height;
        paperSize.height = temp;

        marginRatio = 1;
    }

    //4. 용지영역을 계산한다.
    Long previewAreaWidth = previewArea.right - previewArea.left;
    this->paperArea.left = previewArea.left + (previewAreaWidth / 10 * marginRatio); //왼쪽 여백 20%
    this->paperArea.right = previewArea.right - (previewAreaWidth / 10 * marginRatio); //오른쪽 여백 20%

    Long previewAreaHeight = previewArea.bottom - previewArea.top;
    Long paperAreaPixelHeight = (previewAreaWidth / 10 * (10 - marginRatio * 2)) * paperSize.height / paperSize.width;
    Long heightMargin = previewAreaHeight - paperAreaPixelHeight; //용지 높이를 제외한 여백
    this->paperArea.top = previewArea.top + (heightMargin / 2);
    this->paperArea.bottom = previewArea.bottom - (heightMargin / 2);

    //5. 여백을 읽는다.
    CString left;
    CString right;
    CString up;
    CString down;
    this->parent->GetDlgItem(IDC_EDIT_LEFT)->GetWindowText(left);
    this->parent->GetDlgItem(IDC_EDIT_RIGHT)->GetWindowText(right);
    this->parent->GetDlgItem(IDC_EDIT_UP)->GetWindowText(up);
    this->parent->GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(down);

    //6. 픽셀값으로 환산한다.
    Long paperAreaWidth = this->paperArea.right - this->paperArea.left;
    this->writingArea = this->paperArea;
    this->writingArea.left += atoi(left) * paperAreaWidth / paperSize.width;
    this->writingArea.right -= atoi(right) * paperAreaWidth / paperSize.width;
    this->writingArea.top += atoi(up) * paperAreaWidth / paperSize.width;
    this->writingArea.bottom -= atoi(down) * paperAreaWidth / paperSize.width;

    //7. 클라이언트 좌표계로 수정한다.
    this->parent->ScreenToClient(&this->paperArea);
    this->parent->ScreenToClient(&this->writingArea);
}