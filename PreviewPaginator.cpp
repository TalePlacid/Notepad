#include <afxwin.h>
#include "PreviewPaginator.h"
#include "PreviewForm.h"
#include "NotepadForm.h"
#include "PreviewLayout.h"
#include "PreviewScaler.h"
#include "PagingBuffer.h"
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
}