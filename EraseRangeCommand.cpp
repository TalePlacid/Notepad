#include <afxwin.h>
#include "EraseRangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"
#include "resource.h"
#include "GlyphFactory.h"
#include "ByteChecker.h"
#include "PagingNavigator.h"

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
	if (i > this->frontOffset)
	{
		pagingBuffer->Add(CString("\r\n"));
		this->rearOffset += 2;
	}
	pagingBuffer->Remove(this->rearOffset);
	pagingBuffer->PreviousRow();
	pagingBuffer->Last();

	rowIndex = note->Move(rowIndex);
	row = note->GetAt(rowIndex);
	columnIndex = row->Move(columnIndex);
	
	//4. 적재량이 부족하면 재적재한다.
	if (note->IsBelowBottomLine(rowIndex))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
		if (i > this->frontOffset)
		{
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
			note->MergeRows(rowIndex);
		}
	}
}

void EraseRangeCommand::Undo() {
#if 0
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MoveOffset(this->frontOffset);
	if (!pagingBuffer->IsOnPage(this->frontOffset))
	{
		pagingBuffer->Load();
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->Move(pagingBuffer->GetCurrent().GetColumn());

	MarkingHelper markingHelper(this->parent);
	markingHelper.Mark();

	Long rowCount = 0;
	Glyph* glyph;
	GlyphFactory glyphFactory;
	ByteChecker byteChecker;
	TCHAR character[2];
	Long i = 0;
	while (i < this->contents.GetLength())
	{
		character[0] = this->contents.GetAt(i);
		if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
		{
			character[1] = this->contents.GetAt(++i);
		}

		if (character[0] == '\r' && columnIndex < row->GetLength())
		{
			note->SplitRows(rowIndex, columnIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
			rowCount++;
		}
		else
		{
			if (character[0] != '\r')
			{
				glyph = glyphFactory.Create(character);
				glyph->Select(true);
				row->Add(columnIndex, glyph);
				columnIndex = row->GetCurrent();
			}
			else
			{
				note->SplitRows(rowIndex, columnIndex);
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
				rowCount++;
			}
		}
		i++;
	}

	pagingBuffer->Add(this->contents);

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
#endif
}

void EraseRangeCommand::Redo() {
#if 0
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MoveOffset(this->frontOffset);
	if (!pagingBuffer->IsOnPage(this->frontOffset))
	{
		pagingBuffer->Load();
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->Move(pagingBuffer->GetCurrent().GetColumn());

	TCHAR character[2];
	Long lastIndex = 0;
	Long rowCount = 0;
	Long i = 0;
	while (i < this->contents.GetLength())
	{
		character[0] = this->contents.GetAt(i);
		if (character[0] == '\n')
		{
			lastIndex = i;
			rowCount++;
		}
		i++;
	}

	while (row->GetLength() > columnIndex && row->GetAt(columnIndex)->IsSelected())
	{
		row->Remove(columnIndex);
	}

	i = 0;
	while (i < rowCount - 1)
	{
		note->Remove(rowIndex + 1);
		i++;
	}

	if (rowCount > 0)
	{
		ByteChecker byteChecker;
		Long count = 0;
		i = lastIndex + 1;
		while (i < this->contents.GetLength())
		{
			character[0] = this->contents.GetAt(i);
			if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
			{
				character[1] = this->contents.GetAt(++i);
			}

			count++;
			i++;
		}

		Glyph* nextRow = NULL;
		if (rowIndex + 1 < note->GetLength())
		{
			nextRow = note->GetAt(rowIndex + 1);
		}

		i = 1;
		while (i <= count)
		{
			nextRow->Remove(0);
			i++;
		}

		if (character[0] != '\r' && rowIndex < note->GetLength() - 1)
		{
			note->MergeRows(rowIndex);
		}
	}

	rowIndex = note->Move(rowIndex);
	row = note->GetAt(rowIndex);
	row->Move(columnIndex);

	pagingBuffer->Remove(this->rearOffset);

	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
#endif
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
