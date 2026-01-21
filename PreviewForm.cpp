#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PreviewLayout.h"
#include "PreviewScaler.h"
#include "PreviewPaginator.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "resource.h"

#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnFirstButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnPreviousButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNextButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnLastButtonClicked)
	ON_WM_CLOSE()
	END_MESSAGE_MAP()


PreviewForm::PreviewForm(CWnd *parent) {
	this->parent = parent;
	this->previewLayout = NULL;
	this->previewScaler = NULL;
	this->previewPaginator = NULL;
	this->pageNumberFont = NULL;
}

PreviewForm::~PreviewForm() {
	if (this->previewLayout != NULL)
	{
		delete this->previewLayout;
	}

	if (this->previewScaler != NULL)
	{
		delete this->previewScaler;
	}

	if (this->previewPaginator != NULL)
	{
		delete this->previewPaginator;
	}

	if (this->pageNumberFont != NULL)
	{
		delete this->pageNumberFont;
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

	this->previewPaginator = new PreviewPaginator(this);
	this->previewPaginator->Paginate();

	this->previewScaler = new PreviewScaler(this);
	this->previewScaler->ConvertToPreviewSize();

	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	RECT pageNumberArea = this->previewLayout->GetPageNumberArea();
	this->pageNumber.Create(pageNumber, WS_CHILD | WS_VISIBLE | SS_CENTER, pageNumberArea, this, IDC_STATIC_PAGENUMBER);

	LOGFONT logFont;
	this->previewScaler->GetRegularFont()->GetLogFont(&logFont);
	Long pageNumberHeight = pageNumberArea.bottom - pageNumberArea.top;
	logFont.lfHeight = -pageNumberHeight;
	this->pageNumberFont = new CFont;
	this->pageNumberFont->CreateFontIndirectA(&logFont);
	this->pageNumber.SetFont(this->pageNumberFont);

	return 0;
}

void PreviewForm::OnSize(UINT nType, int cx, int cy) {
	this->previewLayout->Locate();
	this->previewPaginator->Paginate();
	this->previewScaler->ConvertToPreviewSize();

	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	this->pageNumber.SetWindowText(pageNumber);

	LOGFONT logFont;
	this->previewScaler->GetRegularFont()->GetLogFont(&logFont);
	RECT pageNumberArea = this->previewLayout->GetPageNumberArea();
	Long pageNumberHeight = pageNumberArea.bottom - pageNumberArea.top;
	logFont.lfHeight = -pageNumberHeight;
	if (this->pageNumberFont != NULL)
	{
		delete this->pageNumberFont;
	}
	this->pageNumberFont = new CFont;
	this->pageNumberFont->CreateFontIndirectA(&logFont);
	this->pageNumber.SetFont(this->pageNumberFont);
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

	//3. 클리핑 영역을 설정한다.
	RECT writingArea = this->previewLayout->GetWritingArea();
	RECT clipRect;
	clipRect.left = writingArea.left;
	clipRect.right = paperArea.right;
	clipRect.top = paperArea.top;
	clipRect.bottom = paperArea.bottom;
	dc.IntersectClipRect(&clipRect);

	//4. 머리글과 바닥글을 적는다.
	PageSetting pageSetting = ((NotepadForm*)(this->parent))->pageSetting;
	CFont* oldFont = dc.SelectObject(this->previewScaler->GetRegularFont());
	CPoint headerPoint = this->previewLayout->GetHeaderPoint();
	TextOut(dc, headerPoint.x, headerPoint.y, pageSetting.header, pageSetting.header.GetLength());

	CPoint footerPoint = this->previewLayout->GetFooterPoint();
	TextOut(dc, footerPoint.x, footerPoint.y, pageSetting.footer, pageSetting.footer.GetLength());

	//5. 본문 내용을 적는다.
	Long x = writingArea.left;
	Long y = writingArea.top;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long pageStartIndex = (this->previewPaginator->GetCurrent() - 1) * this->previewPaginator->GetRowCountPerPage();

	//큰 글자 영역
	dc.SelectObject(this->previewScaler->GetLargeFont());
	Long largeRowHeight = this->previewScaler->GetLargeRowHeight();
	CString row;
	Long j = 0;
	Long i = pageStartIndex - pagingBuffer->GetRowStartIndex();
	Long nextPageIndex = i + this->previewPaginator->GetRowCountPerPage();
	while (j < this->previewScaler->GetLargeCount() && i < nextPageIndex && i < note->GetLength())
	{
		row.Format("%s", note->GetAt(i)->MakeString().c_str());
		TextOut(dc, x, y, (LPCSTR)row, row.GetLength());
		y += largeRowHeight;
		i++;
		j++;
	}

	//표준 글자 영역
	dc.SelectObject(this->previewScaler->GetRegularFont());
	Long regularRowHeight = this->previewScaler->GetRegularRowHeight();
	while (i < nextPageIndex && i < note->GetLength())
	{
		row.Format("%s", note->GetAt(i)->MakeString().c_str());
		TextOut(dc, x, y, (LPCSTR)row, row.GetLength());
		y += regularRowHeight;
		i++;
	}

#if 0
	Long rowHeight = this->previewScaler->GetRegularRowHeight();
	CString row;
	Long i = pageStartIndex - pagingBuffer->GetRowStartIndex();
	Long nextPageIndex = i + this->previewPaginator->GetRowCountPerPage();
	while (i < nextPageIndex && i < note->GetLength())
	{
		row.Format("%s", note->GetAt(i)->MakeString().c_str());
		TextOut(dc, x, y, (LPCSTR)row, row.GetLength());
		y += rowHeight;
		i++;
	}
#endif
	//6. 쓰기 영역을 그린다.
	CBrush* nullBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen dotPen(PS_DOT, 1, RGB(255, 0, 0)); // 점선, 적색.           
	dc.SelectObject(&dotPen);
	dc.SelectObject(nullBrush);
	dc.Rectangle(&writingArea);

	//7. 페이지바의 컨트롤들을 이동시킨다.
	this->firstButton.MoveWindow(&this->previewLayout->GetFirstButtonArea());
	this->previousButton.MoveWindow(&this->previewLayout->GetPreviousButtonArea());
	this->nextButton.MoveWindow(&this->previewLayout->GetNextButtonArea());
	this->lastButton.MoveWindow(&this->previewLayout->GetLastButtonArea());
	this->pageNumber.MoveWindow(&this->previewLayout->GetPageNumberArea());

	//8. dc 리소스를 해제한다.
	dc.SelectObject(oldBrush);
	dc.SelectObject(oldPen);
	dc.SelectObject(oldFont);
}

void PreviewForm::OnFirstButtonClicked() {
	this->previewPaginator->First();
	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	this->pageNumber.SetWindowText(pageNumber);

	this->Invalidate();
}

void PreviewForm::OnPreviousButtonClicked() {
	this->previewPaginator->Previous();
	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	this->pageNumber.SetWindowText(pageNumber);

	this->Invalidate();
}

void PreviewForm::OnNextButtonClicked() {
	this->previewPaginator->Next();
	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	this->pageNumber.SetWindowText(pageNumber);

	this->Invalidate();
}

void PreviewForm::OnLastButtonClicked() {
	this->previewPaginator->Last();
	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	this->pageNumber.SetWindowText(pageNumber);

	this->Invalidate();
}

void PreviewForm::OnClose() {
	((NotepadForm*)(this->parent))->previewForm = NULL;
	((NotepadForm*)(this->parent))->Notify("UpdateScrollBars");
	this->parent->Invalidate();

	CFrameWnd::OnClose();
}