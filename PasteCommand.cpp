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

	Long last = 0;
	Long count = 0;
	Long i = 0;
	while (i < this->contents.GetLength())
	{
		if (this->contents.GetAt(i) == '\n')
		{
			count++;
			last = i;
		}
		i++;
	}

	ByteChecker byteChecker;
	if (count > 0)
	{
		row->TruncateAfter(columnIndex);
		
		i = 1;
		while (i < count && note->GetLength() - 1 > rowIndex + 1)
		{
			note->Remove(rowIndex+1);
			i++;
		}

		if (rowIndex + 1 < note->GetLength())
		{
			row = note->GetAt(rowIndex + 1);
			Long index = 0;
			i = last + 1;
			while (i < this->contents.GetLength())
			{
				if (byteChecker.IsLeadByte(this->contents.GetAt(i)))
				{
					i++;
				}
				index++;
				i++;
			}

			row->TruncateBefore(index);
			note->MergeRows(rowIndex);
		}
	}
	else
	{
		i = 0;
		while (i < this->contents.GetLength() && row->GetLength() > columnIndex)
		{
			if (byteChecker.IsLeadByte(this->contents.GetAt(i)))
			{
				i++;
			}
			row->Remove(columnIndex);
			i++;
		}
	}
	note->Move(rowIndex);
	row = note->GetAt(rowIndex);
	row->Move(columnIndex);

	pagingBuffer->Remove(this->offset + this->contents.GetLength());
#endif
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