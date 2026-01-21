#include <afxwin.h>
#include <afxdlgs.h>
#include <winspool.h>
#include "PreviewPaginator.h"
#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "Paper.h"
#include "Font.h"
#include "resource.h"

#pragma warning(disable:4996)

PreviewPaginator::PreviewPaginator(CWnd* parent) {
	this->parent = parent;
	this->current = 1;
	this->pageCount = 0;
	this->rowCountPerPage = 0;
}

PreviewPaginator::~PreviewPaginator() {

}

void PreviewPaginator::Paginate() {
	//1. 페이지 설정을 읽는다.
	PageSetting pageSetting = ((NotepadForm*)((PreviewForm*)(this->parent))->parent)->pageSetting;

	//2. 가상 프린터 설정을 가져온다.
	CPrintDialog printDialog(FALSE);
	printDialog.GetDefaults();

	//3. 가상 프린터의 설정을 변경한다.
	DEVMODE* devMode = (DEVMODE*)GlobalLock(printDialog.m_pd.hDevMode);
	devMode->dmPaperSize = Paper::GetDmPaperSize(pageSetting.paperName);
	devMode->dmOrientation = DMORIENT_PORTRAIT;
	if (!pageSetting.isVertical)
	{
		devMode->dmOrientation = DMORIENT_LANDSCAPE;
	}

	devMode->dmFields |= (DM_PAPERSIZE | DM_ORIENTATION); //필드 업데이트 플래그 설정
	HDC printHDC = CreateDC("WINSPOOL", printDialog.GetDeviceName(), NULL, devMode); // 설정 변경
	GlobalUnlock(printDialog.m_pd.hDevMode);

	//4. 프린터 y축 dpi를 구한다.
	CDC dc;
	dc.Attach(printHDC);
	Long dpi = dc.GetDeviceCaps(LOGPIXELSY);

	//5. 물리적인 픽셀 종이 높이를 구한다.
	Long paperHeight = dc.GetDeviceCaps(PHYSICALHEIGHT);

	//6. 여백을 환산한다.
	Long upMargin = pageSetting.margin.up / 25.4 * dpi;
	Long downMargin = pageSetting.margin.down / 25.4 * dpi;

	//7. 쓰기 영역 높이를 구한다.
	Long writingAreaHeight = paperHeight - upMargin - downMargin;

	//8. 줄높이를 구한다.
	CFont* originalFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	if (((NotepadForm*)(((PreviewForm*)(this->parent))->parent))->font != NULL)
	{
		originalFont = ((NotepadForm*)(((PreviewForm*)(this->parent))->parent))->font->GetCFont();
	}

	LOGFONT logFont;
	originalFont->GetLogFont(&logFont);
	Long fontSize = logFont.lfHeight;
	if (fontSize < 0)
	{
		fontSize *= -1;
	}

	Long screenDpi = GetDpiForWindow(this->parent->GetSafeHwnd());
	Long printFontSize = fontSize * dpi / screenDpi;
	logFont.lfHeight = -printFontSize;
	CFont printFont;
	printFont.CreateFontIndirectA(&logFont);

	CFont* oldFont = dc.SelectObject(&printFont);
	
	TEXTMETRIC tm = { 0, };
	dc.GetTextMetrics(&tm);
	Long rowHeight = tm.tmHeight;

	dc.SelectObject(oldFont);
	dc.Detach();
	DeleteDC(printHDC);

	//9. 페이지당 줄 수를 구한다.
	this->rowCountPerPage = writingAreaHeight / rowHeight;

	//10. 페이지 수를 구한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)((PreviewForm*)(this->parent))->parent)->pagingBuffer;
	Long fileRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());
	this->pageCount = fileRowCount / this->rowCountPerPage;
	if (fileRowCount % this->rowCountPerPage > 0)
	{
		(this->pageCount)++;
	}
#if 0
	//1. 페이지 설정을 읽는다.
	PageSetting pageSetting = ((NotepadForm*)((PreviewForm*)(this->parent))->parent)->pageSetting;
	PaperSize paperSize = Paper::GetPaperSize(pageSetting.paperName);
	if (!pageSetting.isVertical)
	{
		Long temp = paperSize.width;
		paperSize.width = paperSize.height;
		paperSize.height = temp;
	}
	Long writingHeight = paperSize.height - pageSetting.margin.up - pageSetting.margin.down;


	//2. mm 줄높이를 구한다.
	SizeCalculator* sizeCalculator = ((NotepadForm*)((PreviewForm*)(this->parent))->parent)->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();

	Long dpi = GetDpiForWindow(this->parent->GetSafeHwnd());
	double mmRowHeight = rowHeight * 25.4 / dpi;

	//3. 페이지당 줄 수를 구한다.
	this->rowCountPerPage = writingHeight / mmRowHeight;

	//4. 페이지 수를 구한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)((PreviewForm*)(this->parent))->parent)->pagingBuffer;
	Long fileRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());
	this->pageCount = fileRowCount / this->rowCountPerPage;
	if (fileRowCount % this->rowCountPerPage > 0)
	{
		(this->pageCount)++;
	}
#endif
#if 0
	//1. 페이지당 줄 수를 구한다.
	PreviewLayout* previewLayout = ((PreviewForm*)(this->parent))->previewLayout;
	PreviewScaler* previewScaler = ((PreviewForm*)(this->parent))->previewScaler;
	RECT writingArea = previewLayout->GetWritingArea();
	Long rowHeight = previewScaler->GetRowHeight();
	Long writingAreaHeight = writingArea.bottom - writingArea.top;
	this->rowCountPerPage = writingAreaHeight / rowHeight;

	//2. 페이지 수를 구한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)((PreviewForm*)(this->parent))->parent)->pagingBuffer;
	Long fileRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());
	this->pageCount = fileRowCount / this->rowCountPerPage;
	if (fileRowCount % this->rowCountPerPage > 0)
	{
		(this->pageCount)++;
	}
#endif
}

Long PreviewPaginator::First() {
	//1. 페이지네이터에서 이동한다.
	Long previous = this->current;
	this->current = 1;

	//2. 페이지네이터에서 이동했다면,
	Long pageStartIndex = -1;
	if (this->current != previous)
	{
		//2.1. 마지막 페이지를 적재한다.
		NotepadForm* notepadForm = (NotepadForm*)((PreviewForm*)(this->parent))->parent;
		SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);

		//2.2. 노트 기준 페이지 시작 줄을 찾는다.
		pageStartIndex = (this->current - 1) * this->rowCountPerPage;

		Glyph* note = notepadForm->note;
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Long rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();

		//2.3. 시작줄이 적재범위보다 아래이면, 재적재한다.
		if (note->IsBelowBottomLine(rowIndex))
		{
			SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();
		}

		//2.4. 노트와 페이징 버퍼에서 이동한다.
		Long currentRowIndex = note->GetCurrent();
		rowIndex = note->Move(rowIndex);
		Glyph* row = note->GetAt(rowIndex);
		row->First();
		Long difference = rowIndex;
		pagingBuffer->PreviousRow(difference);
	}

	return pageStartIndex;
}

Long PreviewPaginator::Previous() {
	//1. 페이지네이터에서 이동한다.
	Long previous = this->current;
	(this->current)--;
	if (this->current < 1)
	{
		this->current = 1;
	}

	//2. 페이지네이터에서 이동했다면,
	Long pageStartIndex = -1;
	if (this->current != previous)
	{
		//2.1. 노트 기준 페이지 시작 줄을 찾는다.
		pageStartIndex = (this->current - 1) * this->rowCountPerPage;

		NotepadForm* notepadForm = (NotepadForm*)((PreviewForm*)(this->parent))->parent;
		Glyph* note = notepadForm->note;
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Long rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();

		//2.2. 시작줄이 적재범위보다 위이면, 재적재한다.
		if (note->IsAboveTopLine(rowIndex))
		{
			SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();
		}

		//2.3. 노트와 페이징 버퍼에서 이동한다.
		Long currentRowIndex = note->GetCurrent();
		rowIndex = note->Move(rowIndex);
		Glyph* row = note->GetAt(rowIndex);
		row->First();
		Long difference = currentRowIndex - rowIndex;
		pagingBuffer->PreviousRow(difference);

		//2.4. 페이지 끝 줄 위치를 구한다.
		Long pageEndIndex = pageStartIndex + this->GetRowCountPerPage();
		Long notePageEndIndex = pageEndIndex - pagingBuffer->GetRowStartIndex();

		//2.5. 페이지 끝 줄 위치가 적재범위에 없다면, 재적재한다.
		if (note->IsBelowBottomLine(notePageEndIndex))
		{
			SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		}
	}

	return pageStartIndex;
}

Long PreviewPaginator::Next() {
	//1. 페이지네이터에서 이동한다.
	Long previous = this->current;
	(this->current)++;
	if (this->current > this->GetPageCount())
	{
		this->current = this->GetPageCount();
	}

	//2. 페이지네이터에서 이동했다면,
	Long pageStartIndex = -1;
	if (this->current != previous)
	{
		//2.1. 노트 기준 페이지 시작 줄을 찾는다.
		pageStartIndex = (this->current - 1) * this->rowCountPerPage;

		NotepadForm* notepadForm = (NotepadForm*)((PreviewForm*)(this->parent))->parent;
		Glyph* note = notepadForm->note;
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Long rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();

		//2.2. 시작줄이 적재범위보다 아래이면, 재적재한다.
		if (note->IsBelowBottomLine(rowIndex))
		{
			SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
			rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();
		}

		//2.3. 노트와 페이징 버퍼에서 이동한다.
		Long currentRowIndex = note->GetCurrent();
		rowIndex = note->Move(rowIndex);
		Glyph* row = note->GetAt(rowIndex);
		row->First();
		Long difference = rowIndex - currentRowIndex;
		pagingBuffer->NextRow(difference);

		//2.4. 페이지 끝 줄 위치를 구한다.
		Long pageEndIndex = pageStartIndex + this->GetRowCountPerPage();
		Long notePageEndIndex = pageEndIndex - pagingBuffer->GetRowStartIndex();

		//2.5. 페이지 끝 줄 위치가 적재범위에 없다면, 재적재한다.
		if (note->IsBelowBottomLine(notePageEndIndex))
		{
			SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		}
	}

	return pageStartIndex;
}

Long PreviewPaginator::Last() {
	//1. 페이지네이터에서 이동한다.
	Long previous = this->current;
	this->current = this->GetPageCount();

	//2. 페이지네이터에서 이동했다면,
	Long pageStartIndex = -1;
	if (this->current != previous)
	{
		//2.1. 마지막 페이지를 적재한다.
		NotepadForm* notepadForm = (NotepadForm*)((PreviewForm*)(this->parent))->parent;
		SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADLAST, 0);

		//2.2. 노트 기준 페이지 시작 줄을 찾는다.
		pageStartIndex = (this->current - 1) * this->rowCountPerPage;

		Glyph* note = notepadForm->note;
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Long rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();

		//2.3. 시작줄이 적재범위보다 위이면, 재적재한다.
		if (note->IsAboveTopLine(rowIndex))
		{
			SendMessage(notepadForm->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
			rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();
		}

		//2.4. 노트와 페이징 버퍼에서 이동한다.
		Long currentRowIndex = note->GetCurrent();
		rowIndex = note->Move(rowIndex);
		Glyph* row = note->GetAt(rowIndex);
		row->First();
		Long difference = currentRowIndex - rowIndex;
		pagingBuffer->PreviousRow(difference);
	}

	return pageStartIndex;
}