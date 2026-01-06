#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ClipboardController.h"
#include "PagingBuffer.h"
#include "resource.h"
#include "ByteChecker.h"
#include "ScrollController.h"
#include "SizeCalculator.h"
#include "RowCounter.h"
#include "PagingNavigator.h"
#include "NoteWrapper.h"

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
	//1. 클립보드에서 붙여넣기가 됐으면,
	BOOL isPasted = ((NotepadForm*)(this->parent))->clipboardController->Paste();
	if (isPasted)
	{
		//1.1. 현재위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		this->contents = ((NotepadForm*)(this->parent))->clipboardController->GetContent();
		this->columnIndex = columnIndex;
		this->offset = pagingBuffer->GetCurrentOffset();

		//1.2. 복사할 내용의 끝까지 반복한다.
		GlyphFactory glyphFactory;
		ByteChecker byteChecker;
		NoteWrapper noteWrapper(this->parent);
		Glyph* glyph;
		TCHAR character[2];
		Long dummied = 0;
		Long i = 0;
		while (i < this->contents.GetLength())
		{
			//1.2.1. 문자를 읽는다.
			character[0] = this->contents.GetAt(i);
			if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
			{
				character[1] = this->contents.GetAt(++i);
			}

			//1.2.2. 줄바꿈 문자가 아니라면,
			if (character[0] != '\r')
			{
				//1.2.2.1. 줄에서 쓴다.
				glyph = glyphFactory.Create(character);
				row->Add(columnIndex, glyph);
				columnIndex = row->GetCurrent();
			}
			else //1.2.3. 줄바꿈 문자라면, 줄을 나눈다.
			{
				note->SplitRows(rowIndex, columnIndex);
				rowIndex = note->Next();
				row = note->GetAt(rowIndex);
				columnIndex = row->First();
			}

			//1.2.3. 자동개행중이면, 재개행한다.
			if (((NotepadForm*)(this->parent))->isAutoWrapped)
			{
				dummied += noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
			i++;
		}

		//1.3. 페이징 버퍼에서 쓴다.
		pagingBuffer->Add(this->contents);

		//1.4. 수직 스크롤바에서 최대값을 조정한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + (RowCounter::CountRow(this->contents) + dummied) * sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}
	}
}

void PasteCommand::Undo() {
	//1. 위치로 이동한다.
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);
	pagingNavigator.NormalizeColumn(this->columnIndex);

	//2. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 컨텐츠 바이트 수만큼 반복한다.
	ByteChecker byteChecker;
	Long merged = 0;
	TCHAR* character;
	Long characterByte;
	Glyph* nextRow;
	Long i = 0;
	while (i < this->contents.GetLength())
	{
		//3.1. 줄의 끝이 아니라면,
		if (columnIndex < row->GetLength())
		{
			//3.1.1. 구한다.
			character = (char*)*row->GetAt(columnIndex);
			characterByte = 1;
			if (byteChecker.IsLeadByte(character[0]))
			{
				characterByte = 2;
			}

			//3.1.2. 노트에서 지운다.
			row->Remove(columnIndex);
			row->Move(columnIndex);
		}
		else //3.2. 줄의 끝이라면,
		{
			//3.2.1. 바이트수를 구한다.
			nextRow = note->GetAt(rowIndex + 1);
			if (!nextRow->IsDummyRow())
			{
				characterByte = 2;
			}
			else
			{
				characterByte = 0;
			}

			//3.2.2. 줄을 합친다.
			note->MergeRows(rowIndex);
			merged++;
		}

		i += characterByte;
	}
	
	//4. 자동개행중이면, 재개행한다.
	Long dummied = 0;
	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		NoteWrapper noteWrapper(this->parent);
		dummied = noteWrapper.Rewrap();
	}

	//5. 페이징버퍼에서 지운다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->Remove(this->offset + this->contents.GetLength());
	pagingBuffer->UnmarkSelectionBegin();

	//6. 수직 스크롤을 조정한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long rowHeight = sizeCalculator->GetRowHeight();
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() - (merged + dummied) * rowHeight;
		scrollController->ResizeVRange(max);
	}

	//7. 적재량이 부족하면, 재적재한다.
	Long pageMax = (pagingBuffer->GetRowStartIndex() + note->GetLength()) * rowHeight;
	if (note->IsBelowBottomLine(rowIndex + 1) && pageMax < scrollController->GetVScroll().GetMax())
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
	}
}

void PasteCommand::Redo() {
	//1. 오프셋 기반으로 이동한다.
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);
	pagingNavigator.NormalizeColumn(this->columnIndex);

	//2. 현재위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//3. 복사할 내용의 끝까지 반복한다.
	GlyphFactory glyphFactory;
	ByteChecker byteChecker;
	NoteWrapper noteWrapper(this->parent);
	Glyph* glyph;
	TCHAR character[2];
	Long dummied = 0;
	Long i = 0;
	while (i < this->contents.GetLength())
	{
		//3.1. 문자를 읽는다.
		character[0] = this->contents.GetAt(i);
		if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
		{
			character[1] = this->contents.GetAt(++i);
		}

		//3.2. 줄바꿈 문자가 아니라면, 줄에서 쓴다.
		if (character[0] != '\r')
		{
			glyph = glyphFactory.Create(character);
			glyph->Select(true);
			row->Add(columnIndex, glyph);
			columnIndex = row->GetCurrent();
		}
		else //3.3. 줄바꿈 문자라면, 줄을 나눈다.
		{
			note->SplitRows(rowIndex, columnIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}

		//3.4. 자동개행중이면, 재개행한다.
		if (((NotepadForm*)(this->parent))->isAutoWrapped)
		{
			dummied += noteWrapper.Rewrap();
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();
		}
		i++;
	}

	//4. 페이징 버퍼에서 쓴다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MarkSelectionBegin();
	pagingBuffer->Add(this->contents);

	//5. 수직 스크롤바에서 최대값을 조정한다.
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	if (scrollController->HasVScroll())
	{
		Scroll vScroll = scrollController->GetVScroll();
		Long max = vScroll.GetMax() + (RowCounter::CountRow(this->contents) + dummied) * sizeCalculator->GetRowHeight();
		scrollController->ResizeVRange(max);
	}
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