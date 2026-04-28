#include <afxwin.h>
#include <afxdlgs.h>
#include "PreviewPaginator.h"
#include "PrinterResource.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"
#include "../PageManager.h"
#include "../CaretNavigator.h"

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
	printDialog.GetDefaults();
	PrinterResource printerResource(this->parent, &printDialog);
	printerResource.LoadMetrics();

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
	//1. 현재 페이지가 첫 페이지가 아니라면,
	if (this->current > 1)
	{
		//1.1. 첫 페이지를 적재한다.
		PageManager::LoadFirst(this->parent);
	}
	else //2. 현재 페이지가 첫 페이지라면,
	{
		//2.1. 첫 위치로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(0);
	}
	this->current = 1;

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
			PageManager::LoadPrevious(this->parent);
			rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();
		}

		//2.3. 노트와 페이징 버퍼에서 이동한다.
		Long currentRowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(currentRowIndex);
		Long currentColumnIndex = row->GetCurrent();
		row->First();

		Long bytes = row->GetPreviousBytes(currentColumnIndex);
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

		while (currentRowIndex > rowIndex)
		{
			currentRowIndex = note->Previous();
			row = note->GetAt(currentRowIndex);
			row->First();

			bytes = row->GetBytes() + 2;
			currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
		}

		//2.4. 페이지 끝 줄 위치를 구한다.
		Long pageEndIndex = pageStartIndex + this->GetRowCountPerPage();
		Long notePageEndIndex = pageEndIndex - pagingBuffer->GetRowStartIndex();

		//2.5. 페이지 끝 줄 위치가 적재범위에 없다면, 재적재한다.
		if (note->IsBelowBottomLine(notePageEndIndex))
		{
			PageManager::LoadNext(this->parent);
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
			PageManager::LoadNext(this->parent);
			rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();
		}

		//2.3. 노트와 페이징 버퍼에서 이동한다.
		Long currentRowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(currentRowIndex);
		Long currentColumnIndex = row->GetCurrent();
		row->First();

		Long bytes = row->GetPreviousBytes(currentColumnIndex);
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

		while (currentRowIndex < rowIndex)
		{
			currentRowIndex = note->Next();
			bytes = row->GetBytes() + 2;
			row = note->GetAt(currentRowIndex);
			row->First();
			currentOffset = pagingBuffer->MoveOffset(currentOffset + bytes);
		}

		//2.4. 페이지 끝 줄 위치를 구한다.
		Long pageEndIndex = pageStartIndex + this->GetRowCountPerPage();
		Long notePageEndIndex = pageEndIndex - pagingBuffer->GetRowStartIndex();

		//2.5. 페이지 끝 줄 위치가 적재범위에 없다면, 재적재한다.
		if (note->IsBelowBottomLine(notePageEndIndex))
		{
			PageManager::LoadNext(this->parent);
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
		PageManager::LoadLast(this->parent);

		//2.2. 노트 기준 페이지 시작 줄을 찾는다.
		pageStartIndex = (this->current - 1) * this->rowCountPerPage;

		Glyph* note = notepadForm->note;
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Long rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();

		//2.3. 시작줄이 적재범위보다 위이면, 재적재한다.
		if (note->IsAboveTopLine(rowIndex))
		{
			PageManager::LoadPrevious(this->parent);
			rowIndex = pageStartIndex - pagingBuffer->GetRowStartIndex();
		}

		//2.4. 노트와 페이징 버퍼에서 이동한다.
		Long currentRowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(currentRowIndex);
		Long currentColumnIndex = row->GetCurrent();
		row->First();

		Long bytes = row->GetPreviousBytes(currentColumnIndex);
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);

		while (currentRowIndex > rowIndex)
		{
			currentRowIndex = note->Previous();
			bytes = row->GetBytes() + 2;
			row = note->GetAt(currentRowIndex);
			row->First();
			currentOffset = pagingBuffer->MoveOffset(currentOffset - bytes);
		}
	}

	return this->current;
}
