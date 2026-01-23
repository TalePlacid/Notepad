#include <afxwin.h>
#include "PreviewPaginator.h"
#include "NotepadForm.h"
#include "PrinterResourceCalculator.h"
#include "PagingBuffer.h"
#include "Glyph.h"
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
	//1. 프린터 리소스를 구한다.
	CPrintDialog printDialog(FALSE);
	PrinterResourceCalculator printerResource(this->parent, &printDialog);
	printerResource.Calculate();

	//2. 페이지당 줄 수를 구한다.
	Margin margin = printerResource.GetPixelMargin();
	Long writingAreaHeight = printerResource.GetPhysicalHeight() - margin.up - margin.down;
	this->rowCountPerPage = writingAreaHeight / printerResource.GetRowHeight();

	//3. 페이지 수를 구한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long fileRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());
	this->pageCount = fileRowCount / this->rowCountPerPage;
	if (fileRowCount % this->rowCountPerPage > 0)
	{
		(this->pageCount)++;
	}
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
		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
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

	return this->current;
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

		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
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

	return this->current;
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

		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
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

	return this->current;
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
		NotepadForm* notepadForm = (NotepadForm*)(this->parent);
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

	return this->current;
}