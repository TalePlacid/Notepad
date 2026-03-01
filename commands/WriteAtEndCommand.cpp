#include "WriteAtEndCommand.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../CaretNavigator.h"
#include "../NoteWrapper.h"
#include "../PageLoader.h"
#include "../Editor.h"

#pragma warning(disable:4996)

WriteAtEndCommand::WriteAtEndCommand(CWnd* parent, const TCHAR(*character), BOOL onChar)
	:Command(parent) {
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->onChar = onChar;
	this->offset = -1;
	this->columnIndex = 0;
	this->isErased = FALSE;
	this->erasedFrontOffset = -1;
	this->erasedRearOffset = -1;
	this->erasedColumnIndex = 0;
	this->erasedContents = "";
}

WriteAtEndCommand::~WriteAtEndCommand() {

}

WriteAtEndCommand::WriteAtEndCommand(const WriteAtEndCommand& source)
	:Command(source.parent) {
	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;
	this->isErased = source.isErased;
	this->erasedFrontOffset = source.erasedFrontOffset;
	this->erasedRearOffset = source.erasedRearOffset;
	this->erasedColumnIndex = source.erasedColumnIndex;
	this->erasedContents = source.erasedContents;
}

WriteAtEndCommand& WriteAtEndCommand::operator=(const WriteAtEndCommand& source) {
	Command::operator=(source);

	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;
	this->isErased = source.isErased;
	this->erasedFrontOffset = source.erasedFrontOffset;
	this->erasedRearOffset = source.erasedRearOffset;
	this->erasedColumnIndex = source.erasedColumnIndex;
	this->erasedContents = source.erasedContents;

	return *this;
}

void WriteAtEndCommand::Execute() {
	this->isErased = FALSE;
	this->erasedFrontOffset = -1;
	this->erasedRearOffset = -1;
	this->erasedColumnIndex = 0;
	this->erasedContents = "";

	if (this->onChar)
	{
		Editor editor(this->parent);
		this->isErased = editor.GetSelectedRange(this->erasedFrontOffset, this->erasedRearOffset);
		if (this->isErased)
		{
			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
			this->erasedContents = pagingBuffer->MakeSelectedString();
			editor.EraseRange(this->erasedFrontOffset, this->erasedRearOffset, this->erasedColumnIndex);
		}
	}

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);
	
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	this->columnIndex = row->GetCurrent();
	
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove();
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (this->character[0] != '\r')
	{
		row->Add(glyph);
		if (this->onChar)
		{
			pagingBuffer->Add((char*)(*glyph));
			this->isUndoable = true;
		}

		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);

			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
		this->columnIndex = row->GetCurrent();
	}
	else
	{
		note->Add(glyph);
		pagingBuffer->Add(this->character);
		this->isUndoable = true;

		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();
}

void WriteAtEndCommand::Undo() {
	//1. 이전 위치로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	this->columnIndex = row->GetCurrent();

	//2. 지운다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (this->columnIndex > 0)
	{
		row->Remove();

		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);

			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
		this->columnIndex = row->GetCurrent();
	}
	else
	{
		if (note->IsAboveTopLine(rowIndex - 1))
		{
			PageLoader::LoadPrevious(this->parent);
		}
		note->Remove();
		rowIndex = note->GetCurrent();
		row = note->GetAt(rowIndex);
		this->columnIndex = row->Last();

		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() - sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Remove();

	this->offset = pagingBuffer->GetCurrentOffset();

	if (this->isErased)
	{
		Editor editor(this->parent);
		editor.InsertTextAt(this->erasedFrontOffset, this->erasedColumnIndex, this->erasedContents, true);
	}
}

void WriteAtEndCommand::Redo() {
	if (this->isErased)
	{
		Editor editor(this->parent);
		editor.EraseRange(this->erasedFrontOffset, this->erasedRearOffset, this->erasedColumnIndex);
	}

	//1. 원래 위치로 이동한다.
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);

	CaretNavigator caretNavigator(parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. 다시 쓴다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	this->columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (this->character[0] != '\r')
	{
		row->Add(glyph);
		pagingBuffer->Add((char*)(*glyph));
	
		if (((NotepadForm*)(this->parent))->IsAutoWrapped())
		{
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);

			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
		this->columnIndex = row->GetCurrent();
	}
	else
	{
		note->Add(glyph);
		TCHAR contents[2];
		contents[0] = '\r';
		contents[1] = '\n';
		pagingBuffer->Add(contents);

		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();
}

Command* WriteAtEndCommand::Clone() {
	return new WriteAtEndCommand(*this);
}
