#include <afxwin.h>
#include <afxdlgs.h>
#include "PrintCommand.h"
#include "SuspendAutoWrap.h"
#include "NotepadForm.h"
#include "PreviewPaginator.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "PageSetting.h"
#include "Font.h"
#include "PrinterResourceCalculator.h"
#include "resource.h"

#pragma warning(disable:4996)

PrintCommand::PrintCommand(CWnd* parent)
	:Command(parent) {
	this->previewPaginator = NULL;
}

PrintCommand::~PrintCommand() {
	if (this->previewPaginator != NULL)
	{
		delete this->previewPaginator;
	}
}

void PrintCommand::Execute() {
	//1. 자동개행을 멈추고, 처음으로 이동한다.
	SuspendAutoWrap suspendAutoWrap(this->parent);
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);

	//2. 프린터 리소스를 읽는다.
	PageSetting pageSetting = ((NotepadForm*)(this->parent))->pageSetting;
	CPrintDialog printDialog(FALSE);
	PrinterResourceCalculator printerResource(this->parent, &printDialog);
	printerResource.Calculate();

	//3. 프린터 DC를 연결한다.
	CDC dc;
	dc.Attach(printDialog.CreatePrinterDC());
	dc.m_bPrinting = TRUE;

	//4. 문서 정보 설정한다.
	DOCINFO di;
	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = "Notepad Document";

	//5. 인쇄한다.
	if (dc.StartDoc(&di) != SP_ERROR) {
		this->previewPaginator = new PreviewPaginator(this->parent);
		this->previewPaginator->Paginate();
			
		CFont* oldFont = dc.SelectObject(printerResource.GetFont());

		//5.1. 페이지 끝까지 반복한다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long pageStartIndex;
		Long rowCountPerPage = this->previewPaginator->GetRowCountPerPage();
		Long nextPageIndex;

		Margin margin = printerResource.GetPixelMargin();
		Long x = margin.left;
		Long y;

		Long j;
		Long previous = 0;
		Long i = this->previewPaginator->GetCurrent();
		while (i <= this->previewPaginator->GetPageCount() && i != previous)
		{
			dc.StartPage();

			//머리글
			y = margin.up / 2;
			TextOut(dc, x, y, pageSetting.header, pageSetting.header.GetLength());

			//본문
			y = margin.up;
			pageStartIndex = (i - 1) * rowCountPerPage;
			j = pageStartIndex - pagingBuffer->GetRowStartIndex();
			nextPageIndex = j + rowCountPerPage;
			while (j < nextPageIndex && j < note->GetLength()) {
				CString row;
				row.Format("%s", note->GetAt(j)->MakeString().c_str());
				dc.TextOut(x, y, (LPCSTR)row, row.GetLength());
				y += printerResource.GetRowHeight();
				j++;
			}

			//바닥글
			y = printerResource.GetPhysicalHeight() - margin.down / 2;
			TextOut(dc, x, y, pageSetting.footer, pageSetting.footer.GetLength());

			dc.EndPage();

			previous = i;
			i = this->previewPaginator->Next();
		}

		dc.EndDoc();
		dc.SelectObject(oldFont);
	}
	dc.Detach();
}