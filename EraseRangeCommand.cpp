#include <afxwin.h>
#include "EraseRangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "resource.h"
#include "GlyphFactory.h"
#include "ByteChecker.h"
#include "PagingNavigator.h"
#include "ScrollController.h"
#include "RowCounter.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

EraseRangeCommand::EraseRangeCommand(CWnd* parent)
	:Command(parent), contents("") {
	this->frontOffset = -1;
	this->rearOffset = -1;
}

EraseRangeCommand::~EraseRangeCommand() {

}

EraseRangeCommand::EraseRangeCommand(const EraseRangeCommand& source)
	:Command(source), contents(source.contents) {
	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
}

EraseRangeCommand& EraseRangeCommand::operator=(const EraseRangeCommand& source) {
	Command::operator=(source);

	this->contents = source.contents;
	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;

	return *this;
}

void EraseRangeCommand::Execute() {	
	//1. 앞 위치로 이동한다.
    PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	this->contents = pagingBuffer->MakeSelectedString();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	if (selectionBeginOffset < currentOffset)
	{
		this->frontOffset = selectionBeginOffset;
		this->rearOffset = currentOffset;
	}
	else
	{
		this->frontOffset = currentOffset;
		this->rearOffset = selectionBeginOffset;
	}

	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->frontOffset);

	//2. 노트에서 선택범위를 삭제한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	ByteChecker byteChecker;
	Glyph* character;
	BOOL hasNextRow = TRUE;
	Long i = this->rearOffset;
	while (hasNextRow && i > this->frontOffset)
	{
		hasNextRow = FALSE;
		while (row->GetLength() > columnIndex && i > this->frontOffset)
		{
			character = row->GetAt(columnIndex);
			i--;
			if (byteChecker.IsLeadByte(*(char*)(*character)))
			{
				i--;
			}
			row->Remove(columnIndex);
		}

		if (rowIndex + 1 < note->GetLength() && i > this->frontOffset)
		{
			hasNextRow = TRUE;
			i -= 2;
			note->MergeRows(rowIndex);
			columnIndex = row->Move(columnIndex);
		}
	}

	//3. 페이징버퍼에서 삭제한다.
	Long rearOffset = this->rearOffset;
	if (i > this->frontOffset)
	{
		pagingBuffer->Add(CString("\r\n"));
		rearOffset += 2;
	}
	pagingBuffer->Remove(rearOffset);
	if (i > this->frontOffset)
	{
		pagingBuffer->PreviousRow();
		pagingBuffer->Last();
	}

	rowIndex = note->Move(rowIndex);
	row = note->GetAt(rowIndex);
	columnIndex = row->Move(columnIndex);
	
	//4. 수직 스크롤바를 반영한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() - RowCounter::CountRow(this->contents) * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}

	//5. 적재량이 부족하면 재적재한다.
	if (note->IsBelowBottomLine(rowIndex))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
		if (i > this->frontOffset)
		{
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
			note->MergeRows(rowIndex);
			row->Move(columnIndex);
		}
	}
}

void EraseRangeCommand::Undo() {
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->frontOffset);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Glyph* glyph;
	ByteChecker byteChecker;
	TCHAR character[2];
	GlyphFactory glyphFactory;
	Long i = 0;
	while (i < this->contents.GetLength())
	{
		character[0] = this->contents[i];
		if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
		{
			character[1] = this->contents[++i];
		}

		if (character[0] != '\r')
		{
			glyph = glyphFactory.Create(character);
			row->Add(columnIndex, glyph);
			columnIndex = row->GetCurrent();
		}
		else
		{
			if (columnIndex < row->GetLength())
			{
				note->SplitRows(rowIndex, columnIndex);
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
			}
			else
			{
				glyph = glyphFactory.Create(character);
				rowIndex = note->Add(rowIndex + 1, glyph);
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
			}
		}
		i++;
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Add(this->contents);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() + RowCounter::CountRow(this->contents) * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}
}   

void EraseRangeCommand::Redo() {
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->frontOffset);

	//2. 노트에서 선택범위를 삭제한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	ByteChecker byteChecker;
	Glyph* character;
	BOOL hasNextRow = TRUE;
	Long i = this->rearOffset;
	while (hasNextRow && i > this->frontOffset)
	{
		hasNextRow = FALSE;
		while (row->GetLength() > columnIndex && i > this->frontOffset)
		{
			character = row->GetAt(columnIndex);
			i--;
			if (byteChecker.IsLeadByte(*(char*)(*character)))
			{
				i--;
			}
			row->Remove(columnIndex);
		}

		if (rowIndex + 1 < note->GetLength() && i > this->frontOffset)
		{
			hasNextRow = TRUE;
			i -= 2;
			note->MergeRows(rowIndex);
			columnIndex = row->Move(columnIndex);
		}
	}

	//3. 페이징버퍼에서 삭제한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long rearOffset = this->rearOffset;
	if (i > this->frontOffset)
	{
		pagingBuffer->Add(CString("\r\n"));
		rearOffset += 2;
	}
	pagingBuffer->Remove(rearOffset);
	if (i > this->frontOffset)
	{
		pagingBuffer->PreviousRow();
		pagingBuffer->Last();
	}

	rowIndex = note->Move(rowIndex);
	row = note->GetAt(rowIndex);
	columnIndex = row->Move(columnIndex);

	//4. 수직 스크롤바를 반영한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() - RowCounter::CountRow(this->contents) * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}

	//5. 적재량이 부족하면 재적재한다.
	if (note->IsBelowBottomLine(rowIndex))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		rowIndex = note->GetCurrent();
		if (i > this->frontOffset)
		{
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
			note->MergeRows(rowIndex);
			row->Move(columnIndex);
		}
	}
}

Command* EraseRangeCommand::Clone() {
	return new EraseRangeCommand(*this);
}

UINT EraseRangeCommand::GetId() {
	return ID_COMMAND_ERASERANGE;
}

bool EraseRangeCommand::IsUndoable() {
	return true;
}
