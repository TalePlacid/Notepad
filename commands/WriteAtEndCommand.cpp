#include "WriteAtEndCommand.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../CaretNavigator.h"
#include "../NoteWrapper.h"

#pragma warning(disable:4996)

WriteAtEndCommand::WriteAtEndCommand(CWnd* parent, char(*character), BOOL onChar)
	:Command(parent) {
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->onChar = onChar;
	this->offset = -1;
}

WriteAtEndCommand::~WriteAtEndCommand() {

}

WriteAtEndCommand::WriteAtEndCommand(const WriteAtEndCommand& source)
	:Command(source.parent) {
	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
}

WriteAtEndCommand& WriteAtEndCommand::operator=(const WriteAtEndCommand& source) {
	Command::operator=(source);

	this->character[0] = const_cast<WriteAtEndCommand&>(source).character[0];
	this->character[1] = const_cast<WriteAtEndCommand&>(source).character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;

	return *this;
}

void WriteAtEndCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);
	
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();
	
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		row->Remove();
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (this->character[0] != '\r')
	{
		columnIndex = row->Add(glyph);
		if (this->onChar)
		{
			pagingBuffer->Add((char*)(*glyph));
		}

		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
	}
	else
	{
		note->Add(glyph);
		pagingBuffer->Add(this->character);

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

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 지운다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (columnIndex > 0)
	{
		row->Remove();

		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
	}
	else
	{
		if (note->IsAboveTopLine(rowIndex - 1))
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADPREVIOUS, 0);
		}
		note->Remove();

		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() - sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (this->onChar)
	{
		pagingBuffer->Remove();
	}

	this->offset = pagingBuffer->GetCurrentOffset();
}

void WriteAtEndCommand::Redo() {
	//1. 원래 위치로 이동한다.
	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->character);

	CaretNavigator caretNavigator(parent);
	caretNavigator.MoveTo(this->offset);

	//2. 다시 쓴다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (this->character[0] != '\r')
	{
		row->Add(glyph);
		if (this->onChar)
		{
			pagingBuffer->Add((char*)(*glyph));
		}

		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			NoteWrapper noteWrapper(this->parent);
			Long dummied = noteWrapper.Rewrap();
			if (scrollController->HasVScroll())
			{
				Scroll vScroll = scrollController->GetVScroll();
				Long max = vScroll.GetMax() + dummied * sizeCalculator->GetRowHeight();
				scrollController->ResizeVRange(max);
			}
		}
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

UINT WriteAtEndCommand::GetId() {
	return ID_COMMAND_WRITEATEND;
}

bool WriteAtEndCommand::IsUndoable() {
	return true;
}