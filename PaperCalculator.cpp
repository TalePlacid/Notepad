#include "PaperCalculator.h"
#include "PreviewForm.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "resource.h"
#include "Paper.h"

#pragma warning(disable:4996)

PaperCalculator::PaperCalculator(CWnd* parent) {
    this->parent = parent;
}

PaperCalculator::~PaperCalculator() {

}

RECT PaperCalculator::CalculatePaperArea() {
    //1. 미리보기 영역을 구한다.
    RECT previewArea;
    CWnd* previewControl = this->parent->GetDlgItem(IDC_GROUP_PREVIEW);
    previewControl->GetWindowRect(&previewArea);
    previewArea.top += CAPTION_SIZE; //캡션 영역 제외.

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
    RECT paperArea;
    Long previewAreaWidth = previewArea.right - previewArea.left;
    paperArea.left = previewArea.left + (previewAreaWidth / 10 * marginRatio); //왼쪽 여백 20%
    paperArea.right = previewArea.right - (previewAreaWidth / 10 * marginRatio); //오른쪽 여백 20%

    Long previewAreaHeight = previewArea.bottom - previewArea.top;
    Long paperAreaPixelHeight = (previewAreaWidth / 10 * (10 - marginRatio * 2)) * paperSize.height / paperSize.width;
    Long heightMargin = previewAreaHeight - paperAreaPixelHeight; //용지 높이를 제외한 여백
    paperArea.top = previewArea.top + (heightMargin / 2);
    paperArea.bottom = previewArea.bottom - (heightMargin / 2);

    return paperArea;
}

RECT PaperCalculator::CalculateWritingArea(RECT paperArea) {
    //1. 여백을 읽는다.
    CString left;
    CString right;
    CString up;
    CString down;
    this->parent->GetDlgItem(IDC_EDIT_LEFT)->GetWindowText(left);
    this->parent->GetDlgItem(IDC_EDIT_RIGHT)->GetWindowText(right);
    this->parent->GetDlgItem(IDC_EDIT_UP)->GetWindowText(up);
    this->parent->GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(down);

    //2. 픽셀값으로 환산한다.
    CString size;
    this->parent->GetDlgItem(IDC_COMBO_SIZE)->GetWindowText(size);
    PaperSize paperSize = Paper::GetPaperSize(size);
    BOOL isVertical = ((CButton*)(this->parent->GetDlgItem(IDC_RADIO_VERTICAL)))->GetCheck();
    BOOL isHorizontal = ((CButton*)(this->parent->GetDlgItem(IDC_RADIO_HORIZONTAL)))->GetCheck();
    Long paperWidth = paperSize.width;
    if (!isVertical && isHorizontal)
    {
        paperWidth = paperSize.height;
    }

    Long paperAreaWidth = paperArea.right - paperArea.left;
    RECT writingArea = paperArea;
    writingArea.left += atoi(left) * paperAreaWidth / paperWidth;
    writingArea.right -= atoi(right) * paperAreaWidth / paperWidth;
    writingArea.top += atoi(up) * paperAreaWidth / paperWidth;
    writingArea.bottom -= atoi(down) * paperAreaWidth / paperWidth;

    return writingArea;
}