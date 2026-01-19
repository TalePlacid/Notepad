#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PreviewLayout.h"
#include "resource.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	END_MESSAGE_MAP()


PreviewForm::PreviewForm(CWnd *parent) {
	this->parent = parent;
	this->previewLayout = NULL;
}

PreviewForm::~PreviewForm() {
	if (this->previewLayout != NULL)
	{
		delete this->previewLayout;
	}
}

int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	this->previewLayout = new PreviewLayout(this);
	this->previewLayout->Locate();

	this->firstButton.Create("<<", WS_CHILD | WS_VISIBLE | SS_CENTER, this->previewLayout->GetFirstButtonArea(), this, IDC_BUTTON_FIRST);
	this->previousButton.Create("<", WS_CHILD | WS_VISIBLE | SS_CENTER, this->previewLayout->GetPreviousButtonArea(), this, IDC_BUTTON_PREVIOUS);
	this->nextButton.Create(">", WS_CHILD | WS_VISIBLE | SS_CENTER, this->previewLayout->GetNextButtonArea(), this, IDC_BUTTON_NEXT);
	this->lastButton.Create(">>", WS_CHILD | WS_VISIBLE | SS_CENTER, this->previewLayout->GetLastButtonArea(), this, IDC_BUTTON_LAST);

	this->pageNumber.Create("000 / 000", WS_CHILD | WS_VISIBLE | SS_CENTER, this->previewLayout->GetPageNumberArea(), this, IDC_STATIC_PAGENUMBER);

	return 0;
}

void PreviewForm::OnSize(UINT nType, int cx, int cy) {
	this->previewLayout->Locate();
}

void PreviewForm::OnPaint() {
	CPaintDC dc(this);

	//1. 그림자를 그린다.
	CBrush blackBrush(RGB(0, 0, 0));
	CBrush* oldBrush = dc.SelectObject(&blackBrush);
	RECT paperArea = this->previewLayout->GetPaperArea();
	dc.Rectangle(paperArea.left + 5, paperArea.top + 5, paperArea.right + 5, paperArea.bottom + 5);

	//2. 종이영역을 그린다.
	CPen solidPen(PS_SOLID, 1, RGB(0, 0, 0));   // 검은색 1픽셀 펜
	CPen* oldPen = dc.SelectObject(&solidPen);
	CBrush whiteBrush(RGB(255, 255, 255));
	dc.SelectObject(&whiteBrush);
	dc.Rectangle(&paperArea);

	//3. 쓰기 영역을 그린다.
	CBrush* nullBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen dotPen(PS_DOT, 1, RGB(255, 0, 0)); // 점선, 적색.           
	dc.SelectObject(&dotPen);
	dc.SelectObject(nullBrush);
	dc.Rectangle(&this->previewLayout->GetWritingArea());

	//4. 페이지바의 컨트롤들을 이동시킨다.
	this->firstButton.MoveWindow(&this->previewLayout->GetFirstButtonArea());
	this->previousButton.MoveWindow(&this->previewLayout->GetPreviousButtonArea());
	this->nextButton.MoveWindow(&this->previewLayout->GetNextButtonArea());
	this->lastButton.MoveWindow(&this->previewLayout->GetLastButtonArea());
	this->pageNumber.MoveWindow(&this->previewLayout->GetPageNumberArea());

	//5. dc 리소스를 해제한다.
	dc.SelectObject(oldBrush);
	dc.SelectObject(oldPen);
}

void PreviewForm::OnClose() {
	((NotepadForm*)(this->parent))->previewForm = NULL;

	CFrameWnd::OnClose();
}