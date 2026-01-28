#include <afxwin.h>
#include <afxdlgs.h>
#include "PrinterResource.h"
#include "NotepadForm.h"
#include "Paper.h"

#pragma warning(disable:4996)

PrinterResource::PrinterResource(CWnd* parent, CPrintDialog* printDialog) {
	this->parent = parent;
	this->printDialog = printDialog;
	this->font = NULL;
	this->dpiX = 0;
	this->dpiY = 0;
	this->physicalWidth = 0;
	this->physicalHeight = 0;
	this->rowHeight = 0;
}

PrinterResource::~PrinterResource() {
	if (this->font != NULL)
	{
		delete this->font;
	}
}

void PrinterResource::LoadMetrics() {
	//1. 페이지 설정을 읽는다.
	PageSetting pageSetting = ((NotepadForm*)(this->parent))->pageSetting;

	//2. 프린터 설정을 변경한다.
	DEVMODE* devMode = (DEVMODE*)GlobalLock(this->printDialog->m_pd.hDevMode);
	devMode->dmPaperSize = Paper::GetDmPaperSize(pageSetting.paperName);
	devMode->dmOrientation = DMORIENT_PORTRAIT;
	if (!pageSetting.isVertical)
	{
		devMode->dmOrientation = DMORIENT_LANDSCAPE;
	}

	devMode->dmFields |= (DM_PAPERSIZE | DM_ORIENTATION); //필드 업데이트 플래그 설정
	HDC printHDC = CreateDC("WINSPOOL", this->printDialog->GetDeviceName(), NULL, devMode); // 설정 변경
	GlobalUnlock(this->printDialog->m_pd.hDevMode);

	//3. 프린터 dpi를 구한다.
	CDC dc;
	dc.Attach(printHDC);
	this->dpiX = dc.GetDeviceCaps(LOGPIXELSX);
	this->dpiY = dc.GetDeviceCaps(LOGPIXELSY);

	//4. 물리적인 픽셀 종이 수치들을 구한다.
	this->physicalWidth = dc.GetDeviceCaps(PHYSICALWIDTH);
	this->physicalHeight = dc.GetDeviceCaps(PHYSICALHEIGHT);

	//5. 여백을 환산한다.
	this->pixelMargin.left = pageSetting.margin.left / 25.4 * this->dpiX;
	this->pixelMargin.right = pageSetting.margin.right / 25.4 * this->dpiX;
	this->pixelMargin.up = pageSetting.margin.up / 25.4 * this->dpiY;
	this->pixelMargin.down = pageSetting.margin.down / 25.4 * this->dpiY;

	//6. 쓰기 영역 높이를 구한다.
	Long writingAreaHeight = this->physicalHeight - this->pixelMargin.up - this->pixelMargin.down;

	//7. 폰트를 구한다.
	CFont* originalFont = ((NotepadForm*)(this->parent))->displayFont;

	LOGFONT logFont;
	originalFont->GetLogFont(&logFont);
	Long fontSize = logFont.lfHeight;
	if (fontSize < 0)
	{
		fontSize *= -1;
	}

	Long screenDpi = GetDpiForWindow(this->parent->GetSafeHwnd());
	Long printFontSize = fontSize * this->dpiY / screenDpi;
	logFont.lfHeight = -printFontSize;

	if (this->font != NULL)
	{
		delete this->font;
		this->font = NULL;
	}
	this->font = new CFont;
	this->font->CreateFontIndirectA(&logFont);

	CFont* oldFont = dc.SelectObject(this->font);

	//8. 줄높이를 구한다.
	TEXTMETRIC tm = { 0, };
	dc.GetTextMetrics(&tm);
	this->rowHeight = tm.tmHeight;

	dc.SelectObject(oldFont);
	dc.Detach();
	DeleteDC(printHDC);
}