#include "WriteAtEndCommand.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "PagingNavigator.h"
#include "resource.h"

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
	
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long max = scrollController->GetHScroll().GetMax();
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowWidth = sizeCalculator->GetRowWidth(row->MakeString().c_str());
	if (((NotepadForm*)(this->parent))->IsCompositing())
	{
		if (scrollController->HasHScroll() && rowWidth == max)
		{
			max -= sizeCalculator->GetCharacterWidth((char*)(*(row->GetAt(columnIndex - 1))));
			scrollController->ResizeHRange(max);
		}
		row->Remove();
	}

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (this->character[0] != '\r')
	{
		row->Add(glyph);
		if (this->onChar)
		{
			pagingBuffer->Add((char*)(*glyph));
		}

		rowWidth = sizeCalculator->GetRowWidth(row->MakeString().c_str());
		if (scrollController->HasHScroll() && rowWidth > max)
		{
			max = rowWidth;
			scrollController->ResizeHRange(max);
		}
	}
	else
	{
		note->Add(glyph);
		TCHAR contents[2];
		contents[0] = '\r';
		contents[1] = '\n';
		pagingBuffer->Add(contents);

		if (scrollController->HasVScroll())
		{
			max = scrollController->GetVScroll().GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
			scrollController->Down();
		}
	}
	this->offset = pagingBuffer->GetCurrentOffset();
}

void WriteAtEndCommand::Undo() {
	//1. 이전 위치로 이동한다.
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 지운다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long max = scrollController->GetHScroll().GetMax();
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowWidth = sizeCalculator->GetRowWidth(row->MakeString().c_str());
	if (columnIndex > 0)
	{
		if (scrollController->HasHScroll() && rowWidth == max)
		{
			max -= sizeCalculator->GetCharacterWidth(this->character);
			scrollController->ResizeHRange(max);
		}
		row->Remove();
	}
	else
	{
		if (scrollController->HasVScroll())
		{
			max = scrollController->GetVScroll().GetMax();
			scrollController->ResizeVRange(max - sizeCalculator->GetRowHeight());
			scrollController->Up();
		}
		note->Remove();
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

	PagingNavigator pagingNavigator(parent);
	pagingNavigator.MoveTo(this->offset);

	//2. 다시 쓴다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	Long max = scrollController->GetHScroll().GetMax();
	if (this->character[0] != '\r')
	{
		row->Add(glyph);
		if (this->onChar)
		{
			pagingBuffer->Add((char*)(*glyph));
		}

		Long rowWidth = sizeCalculator->GetRowWidth(row->MakeString().c_str());
		if (scrollController->HasHScroll() && rowWidth > max)
		{
			max = rowWidth;
			scrollController->ResizeHRange(max);
		}
	}
	else
	{
		note->Add(glyph);
		TCHAR contents[2];
		contents[0] = '\r';
		contents[1] = '\n';
		pagingBuffer->Add(contents);

		if (scrollController->HasVScroll())
		{
			max = scrollController->GetVScroll().GetMax() + sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
			scrollController->Down();
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