#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ClipboardController.h"
#include "PagingBuffer.h"
#include "resource.h"
#include "ByteChecker.h"
#include "MarkingHelper.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "RowCounter.h"
#include "PagingNavigator.h"

#pragma warning(disable:4996)

PasteCommand::PasteCommand(CWnd* parent)
	:Command(parent), contents("") {
	this->offset = -1;
}

PasteCommand::~PasteCommand() {

}

PasteCommand::PasteCommand(const PasteCommand& source)
	:Command(source), contents(source.contents) {
	this->offset = source.offset;
}

PasteCommand& PasteCommand::operator=(const PasteCommand& source) {
	Command::operator=(source);

	this->contents = source.contents;
	this->offset = source.offset;

	return *this;
}

void PasteCommand::Execute() {
	BOOL isPasted = ((NotepadForm*)(this->parent))->clipboardController->Paste();
	if (isPasted)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		this->contents = ((NotepadForm*)(this->parent))->clipboardController->GetContent();
		this->offset = pagingBuffer->GetCurrentOffset();

		MarkingHelper markingHelper(this->parent);
		markingHelper.Mark();

		TCHAR letters[2];
		Glyph* glyph;
		GlyphFactory glyphFactory;
		ByteChecker byteChecker;
		Long length = strlen((LPCTSTR)(this->contents));
		Long i = 0;
		while (i < length)
		{
			letters[0] = this->contents.GetAt(i);
			if (byteChecker.IsLeadByte(letters[0]) || letters[0] == '\r')
			{
				letters[1] = this->contents.GetAt(++i);
			}

			if (letters[0] != '\r')
			{
				glyph = glyphFactory.Create(letters);
				glyph->Select(true);
				columnIndex = row->Add(columnIndex, glyph);
			}
			else
			{
				note->SplitRows(rowIndex, columnIndex);
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
				
			}
			columnIndex = row->GetCurrent();
			i++;
		}

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
}

void PasteCommand::Undo() {
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	Long rearOffset = this->offset + this->contents.GetLength();

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	ByteChecker byteChecker;
	Glyph* character;
	BOOL hasNextRow = TRUE;
	Long i = rearOffset;
	while (hasNextRow && i > this->offset)
	{
		hasNextRow = FALSE;
		while (row->GetLength() > columnIndex && i > this->offset)
		{
			character = row->GetAt(columnIndex);
			i--;
			if (byteChecker.IsLeadByte(*(char*)(*character)))
			{
				i--;
			}
			row->Remove(columnIndex);
		}

		if (rowIndex + 1 < note->GetLength() && i > this->offset)
		{
			hasNextRow = TRUE;
			i -= 2;
			note->MergeRows(rowIndex);
			columnIndex = row->Move(columnIndex);
		}
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (i > this->offset)
	{
		pagingBuffer->Add(CString("\r\n"));
		rearOffset += 2;
	}
	pagingBuffer->Remove(rearOffset);
	if (i > this->offset)
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
		if (i > this->offset)
		{
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
			note->MergeRows(rowIndex);
			row->Move(columnIndex);
		}
	}
}

void PasteCommand::Redo() {
#if 0
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MoveOffset(this->offset);
	if (!pagingBuffer->IsOnPage(this->offset))
	{
		pagingBuffer->Load();
	}

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->Move(pagingBuffer->GetCurrent().GetColumn());

	MarkingHelper markingHelper(this->parent);
	markingHelper.Mark();

	TCHAR letters[2];
	Glyph* glyph;
	GlyphFactory glyphFactory;
	ByteChecker byteChecker;
	Long length = strlen((LPCTSTR)(this->contents));
	Long i = 0;
	while (i < length)
	{
		letters[0] = this->contents.GetAt(i);
		if (byteChecker.IsLeadByte(letters[0]) || letters[0] == '\r')
		{
			letters[1] = this->contents.GetAt(++i);
		}

		if (letters[0] != '\r')
		{
			glyph = glyphFactory.Create(letters);
			glyph->Select(true);
			columnIndex = row->Add(columnIndex, glyph);
			pagingBuffer->Add((char*)(*glyph));
		}
		else
		{
			note->SplitRows(rowIndex, columnIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			pagingBuffer->Add(letters);
		}
		columnIndex = row->GetCurrent();
		i++;
	}

	if (!pagingBuffer->IsAboveBottomLine())
	{
		pagingBuffer->Load();
	}

	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
#endif
}

Command* PasteCommand::Clone() {
	return new PasteCommand(*this);
}

UINT PasteCommand::GetId() {
	return ID_COMMAND_PASTE;
}

bool PasteCommand::IsUndoable() {
	return true;
}