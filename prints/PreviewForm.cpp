#include <afxdlgs.h>
#include "PreviewForm.h"
#include "../resource.h"
#include "PrinterResource.h"
#include "PreviewLayout.h"
#include "PreviewPaginator.h"
#include "PrintRenderer.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"

#pragma warning(disable:4996)

class DebugWindow : public CFrameWnd {
public:
     CBitmap * m_pBitmap; // 확인할 비트맵
     int m_nWidth;
     int m_nHeight;

     DebugWindow(CBitmap * pBitmap, int w, int h) {
         m_pBitmap = pBitmap;
         m_nWidth = w;
         m_nHeight = h;
         // 윈도우 생성 (제목: MEMDC DEBUG, 크기는 화면 절반 정도)
         Create(NULL, "MEMDC DEBUG", WS_OVERLAPPEDWINDOW, CRect(100, 100,
				800, 800));
	}

     afx_msg void OnPaint() {
         CPaintDC dc(this);
         if (!m_pBitmap) return;

         // 전달받은 비트맵을 그린다.
         CDC memDC;
         memDC.CreateCompatibleDC(&dc);
         CBitmap * pOld = memDC.SelectObject(m_pBitmap);

         // 1:1로 그리면 너무 커서 안 보일 수 있으니,
         // 스크롤 기능 대신 StretchBlt로 윈도우 크기에 맞춰 축소해서 보여줌
         CRect clientRect;
         GetClientRect(&clientRect);

         dc.SetStretchBltMode(HALFTONE);
         dc.StretchBlt(0, 0, clientRect.Width(), clientRect.Height(),
			 & memDC, 0, 0, m_nWidth, m_nHeight, SRCCOPY);

        memDC.SelectObject(pOld);

	}
    DECLARE_MESSAGE_MAP()
};

 BEGIN_MESSAGE_MAP(DebugWindow, CFrameWnd)
     ON_WM_PAINT()
 END_MESSAGE_MAP()



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
	this->previewPaginator = NULL;
	this->pageNumberFont = NULL;
}

PreviewForm::~PreviewForm() {
	if (this->previewLayout != NULL)
	{
		delete this->previewLayout;
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

	this->previewPaginator = new PreviewPaginator(this->parent);
	this->previewPaginator->Paginate();

	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	RECT pageNumberArea = this->previewLayout->GetPageNumberArea();
	this->pageNumber.Create(pageNumber, WS_CHILD | WS_VISIBLE | SS_CENTER, pageNumberArea, this, IDC_STATIC_PAGENUMBER);

	LOGFONT logFont;
	CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))->GetLogFont(&logFont);
	Long pageNumberHeight = pageNumberArea.bottom - pageNumberArea.top;
	logFont.lfHeight = -pageNumberHeight;
	this->pageNumberFont = new CFont;
	this->pageNumberFont->CreateFontIndirect(&logFont);
	this->pageNumber.SetFont(this->pageNumberFont);

	return 0;
}

void PreviewForm::OnSize(UINT nType, int cx, int cy) {
	this->previewLayout->Locate();
	this->previewPaginator->Paginate();

	CString pageNumber;
	pageNumber.Format("%03ld / %03ld", this->previewPaginator->GetCurrent(), this->previewPaginator->GetPageCount());
	this->pageNumber.SetWindowText(pageNumber);

	LOGFONT logFont;
	CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))->GetLogFont(&logFont);
	RECT pageNumberArea = this->previewLayout->GetPageNumberArea();
	Long pageNumberHeight = pageNumberArea.bottom - pageNumberArea.top;
	logFont.lfHeight = -pageNumberHeight;
	if (this->pageNumberFont != NULL)
	{
		delete this->pageNumberFont;
	}
	this->pageNumberFont = new CFont;
	this->pageNumberFont->CreateFontIndirect(&logFont);
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
	CBrush whiteBrush(RGB(255, 255, 255));
	dc.SelectObject(&whiteBrush);
	dc.Rectangle(&paperArea);

	//3. 프린터 자원을 읽는다.
	CPrintDialog printDialog(FALSE);
	printDialog.GetDefaults();

	PrinterResource printerResource(this->parent, &printDialog);
	printerResource.LoadMetrics();
	Long printWidth = printerResource.GetPhysicalWidth();
	Long printHeight = printerResource.GetPhysicalHeight();

	//4. 프린터 DC를 만든다.
	CDC printerDC;
	printerDC.Attach(printDialog.CreatePrinterDC());

	//5. 대체 DC를 만든다.
	CDC memDC;
	memDC.CreateCompatibleDC(&printerDC);

	//DIB 헤더 설정	
	BITMAPINFO bitmapInfo;
	ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = printWidth;
	bitmapInfo.bmiHeader.biHeight = -printHeight; // Top-down 방식
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24; // 24비트 트루컬러 (32비트보다 25% 절약)
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	//DIB Section 생성(시스템 힙 사용)
	void* pBits = NULL;
	HBITMAP hDib = CreateDIBSection(dc.GetSafeHdc(), &bitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

	CBitmap bitmap;
	bitmap.Attach(hDib);

	CBitmap* oldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, printWidth, printHeight, WHITENESS);
	CFont* oldFont = memDC.SelectObject(printerResource.GetFont());

	//6. 대체 비트맵에 그린다.
	PrintRenderer printRenderer(this->parent, this->previewPaginator, &printerResource);
	printRenderer.Render(&memDC);
	 
	//7. 대체 비트맵을 축소해 붙인다.
	Long paperWidth = paperArea.right - paperArea.left;
	Long paperHeight = paperArea.bottom - paperArea.top;
	
	dc.SetStretchBltMode(HALFTONE);
	StretchDIBits(dc.GetSafeHdc(),
			paperArea.left, paperArea.top, paperWidth, paperHeight,
			0, 0, printWidth, printHeight,
			pBits, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	//8. 대체 비트맵 관련 리소스들을 해제한다.
	memDC.SelectObject(oldFont);
	memDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
	printerDC.Detach();

	//9. 종이 외곽선 그리기
	CPen solidPen(PS_SOLID, 1, RGB(0, 0, 0));   // 검은색 1픽셀 펜
	CBrush* nullBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldPen = dc.SelectObject(&solidPen);
	dc.SelectObject(nullBrush);
	dc.Rectangle(&paperArea);

	//10. 쓰기 영역을 그린다.
	RECT writingArea = this->previewLayout->GetWritingArea();
	CPen dotPen(PS_DOT, 1, RGB(255, 0, 0)); // 점선, 적색.           
	dc.SelectObject(&dotPen);
	dc.Rectangle(&writingArea);

	//11. 페이지바의 컨트롤들을 이동시킨다.
	this->firstButton.MoveWindow(&this->previewLayout->GetFirstButtonArea());
	this->previousButton.MoveWindow(&this->previewLayout->GetPreviousButtonArea());
	this->nextButton.MoveWindow(&this->previewLayout->GetNextButtonArea());
	this->lastButton.MoveWindow(&this->previewLayout->GetLastButtonArea());
	this->pageNumber.MoveWindow(&this->previewLayout->GetPageNumberArea());

	//12. dc 리소스를 해제한다.
	dc.SelectObject(oldBrush);
	dc.SelectObject(oldPen);
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

