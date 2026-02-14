#include <afxwin.h>
#include "PrintRenderer.h"
#include "PrinterResource.h"
#include "PageSetting.h"
#include "PreviewPaginator.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"

#pragma warning(disable:4996)

PrintRenderer::PrintRenderer(CWnd* parent, PreviewPaginator* paginator, 
    PrinterResource* printerResource) {
    this->parent = parent;
	this->paginator = paginator;
    this->printerResource = printerResource;
}

PrintRenderer::~PrintRenderer() {

}

void PrintRenderer::Render(CDC* dc) {
    //1. 페이지 설정을 읽는다.
    PageSetting pageSetting = ((NotepadForm*)(this->parent))->pageSetting;

    //2. 머리글을 쓴다.
    Margin margin = this->printerResource->GetPixelMargin();
    Long x = margin.left;
    Long y = margin.up / 2;
    dc->SetTextColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);
    dc->TextOut(x, y, pageSetting.header, pageSetting.header.GetLength());

    //3. 본문을 쓴다.
    PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
    Glyph* note = ((NotepadForm*)(this->parent))->note;
    CString row;
    y = margin.up;
    Long rowHeight = this->printerResource->GetRowHeight();
    Long rowCountPerPage = this->paginator->GetRowCountPerPage();
    Long current = this->paginator->GetCurrent();
    Long pageStartIndex = (current - 1) * rowCountPerPage;
    Long i = pageStartIndex - pagingBuffer->GetRowStartIndex();
    Long nextPageIndex = i + rowCountPerPage;
    while (i < nextPageIndex && i < note->GetLength()) {
        row.Format("%s", note->GetAt(i)->MakeString().c_str());
        dc->TextOut(x, y, (LPCSTR)row, row.GetLength());
        y += rowHeight;
        i++;
    }

    //4. 바닥글을 쓴다.
    y = (this->printerResource->GetPhysicalHeight() - margin.down / 2);
    dc->TextOut(x, y, pageSetting.footer, pageSetting.footer.GetLength());
}