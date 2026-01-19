#include "PreviewLayout.h"
#include "PreviewForm.h"
#include "NotepadForm.h"
#include "Paper.h";

#pragma warning(disable:4996)

PreviewLayout::PreviewLayout(CWnd* parent) {
	this->parent = parent;
}

PreviewLayout::~PreviewLayout() {

}

void PreviewLayout::Locate() {
	//1. 페이지 설정을 읽는다.
	PreviewForm* previewForm = (PreviewForm*)(this->parent);
	PageSetting pageSetting = ((NotepadForm*)(previewForm->parent))->pageSetting;
	PaperSize paperSize = Paper::GetPaperSize(pageSetting.paperName);
	if (!pageSetting.isVertical)
	{
		Long temp = paperSize.width;
		paperSize.width = paperSize.height;
		paperSize.height = temp;
	}

	//2. 클라이언트 영역의 크기를 잰다.
	GetClientRect(this->parent->GetSafeHwnd(), &this->clientArea);
	Long clientAreaHeight = this->clientArea.bottom - this->clientArea.top;
	Long clientAreaWidth = this->clientArea.right - this->clientArea.left;

	//3. 종이 영역을 구한다.
	this->paperArea.top = clientAreaHeight * 0.1 + 0.5; // 상단10% 아래 지점
	this->paperArea.bottom = clientAreaHeight * 0.85 + 0.5;  // 하단15% 위 지점
	Long paperAreaHeight = this->paperArea.bottom - this->paperArea.top;

	double scale = paperAreaHeight / (paperSize.height * 1.0);
	Long paperAreaWidth = paperSize.width * scale + 0.5;
	Long paperAreaMargin = (clientAreaWidth - paperAreaWidth) / 2;
	this->paperArea.left = paperAreaMargin;
	this->paperArea.right = clientAreaWidth - paperAreaMargin;

	//3. 쓰기 영역을 구한다.
	Long leftMargin = pageSetting.margin.left * scale + 0.5;
	Long rightMargin = pageSetting.margin.right * scale + 0.5;
	Long upMargin = pageSetting.margin.up * scale + 0.5;
	Long downMargin = pageSetting.margin.down * scale + 0.5;
	
	this->writingArea.left = paperArea.left + leftMargin;
	this->writingArea.right = paperArea.right - rightMargin;
	this->writingArea.top = paperArea.top + upMargin;
	this->writingArea.bottom = paperArea.bottom - downMargin;

	//4. 페이지 바의 좌표를 구한다.
	Long shadowAreaBottom = (this->paperArea.bottom + 5);
	Long clientAreaDownMargin = this->clientArea.bottom - shadowAreaBottom;
	Long pageBarTop = shadowAreaBottom + clientAreaDownMargin / 3;
	Long pageBarBottom = this->clientArea.bottom - clientAreaDownMargin / 3;
	Long pageBarHeight = pageBarBottom - pageBarTop;
	
	//5. 각 버튼의 좌료를 구한다.
	Long buttonWidth = pageBarHeight;
	Long buttonMargin = pageBarHeight / 2;

	this->firstButtonArea.left = paperArea.left;
	this->firstButtonArea.right = this->firstButtonArea.left + buttonWidth;
	this->firstButtonArea.top = pageBarTop;
	this->firstButtonArea.bottom = pageBarBottom;

	this->previousButtonArea.left = this->firstButtonArea.right + buttonMargin;
	this->previousButtonArea.right = this->previousButtonArea.left + buttonWidth;
	this->previousButtonArea.top = pageBarTop;
	this->previousButtonArea.bottom = pageBarBottom;

	this->lastButtonArea.left = this->paperArea.right - buttonWidth;
	this->lastButtonArea.right = this->paperArea.right;
	this->lastButtonArea.top = pageBarTop;
	this->lastButtonArea.bottom = pageBarBottom;

	this->nextButtonArea.left = this->lastButtonArea.left - buttonWidth - buttonMargin;
	this->nextButtonArea.right = this->nextButtonArea.left + buttonWidth;
	this->nextButtonArea.top = pageBarTop;
	this->nextButtonArea.bottom = pageBarBottom;

	//6. 페이지 넘버 정적 영역을 구한다.
	this->pageNumberArea.left = this->previousButtonArea.right + buttonMargin;
	this->pageNumberArea.right = this->nextButtonArea.left - buttonMargin;
	this->pageNumberArea.top = pageBarTop;
	this->pageNumberArea.bottom = pageBarBottom;
}