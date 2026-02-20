#include "PasteCommand.h"
#include "../Editor.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../ClipboardController.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../ByteChecker.h"
#include "../RowCounter.h"
#include "../CaretNavigator.h"
#include "../NoteWrapper.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

PasteCommand::PasteCommand(CWnd* parent)
	:Command(parent), erased(""), contents("") {
	this->frontOffset = -1;
	this->rearOffset = -1;
	this->columnIndex = 0;
}

PasteCommand::~PasteCommand() {

}

PasteCommand::PasteCommand(const PasteCommand& source)
	:Command(source), contents(source.contents) {
	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;
	this->erased = source.erased;
	this->contents = source.contents;
}

PasteCommand& PasteCommand::operator=(const PasteCommand& source) {
	Command::operator=(source);

	this->frontOffset = source.frontOffset;
	this->rearOffset = source.rearOffset;
	this->columnIndex = source.columnIndex;
	this->erased = source.erased;
	this->contents = source.contents;

	return *this;
}

void PasteCommand::Execute() {
	//1. 클립보드에서 붙여넣기가 됐으면,
	ClipboardController* clipboardController = ((NotepadForm*)(this->parent))->clipboardController;
	BOOL isPasted = clipboardController->Paste();
	if (isPasted)
	{
		//2. 선택범위가 있으면, 지운다.
		Editor editor(this->parent);
		BOOL isSelected = editor.GetSelectedRange(frontOffset, rearOffset);
		if (isSelected)
		{
			editor.EraseRange(frontOffset, rearOffset, this->columnIndex, this->erased);
		}

		//3. 내용을 붙여넣는다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		this->frontOffset = pagingBuffer->GetCurrentOffset();
		this->contents = clipboardController->GetContent();
		editor.InsertTextAt(pagingBuffer->GetCurrentOffset(), this->columnIndex, this->contents);
	}
}

void PasteCommand::Undo() {
#if 0
	//1. 위치로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

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
	BOOL flag = TRUE;
	while (i < this->contents.GetLength() && flag)
	{
		flag = FALSE;
		//3.1. 줄의 끝이 아니라면,
		if (columnIndex < row->GetLength())
		{
			flag = TRUE;
			//3.1.1. 바이트수를 구한다.
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
		else if (rowIndex + 1 < note->GetLength())//3.2. 줄의 끝이고 다음 줄이 있다면,
		{
			flag = TRUE;
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
	if (note->IsBelowBottomLine(rowIndex + 1))
	{
		if (!row->IsDummyRow() && columnIndex == 0)
		{
			pagingBuffer->Add(CString("\r\n"));
			pagingBuffer->PreviousRow();
		}
		PageLoader::LoadNext(this->parent);
		if (!row->IsDummyRow() && columnIndex == 0)
		{
			note->MergeRows(rowIndex);
			pagingBuffer->NextRow();
			pagingBuffer->Remove();
		}
	}
#endif
}

void PasteCommand::Redo() {
#if 0
	//1. 오프셋 기반으로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

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
			glyph = glyphFactory.Create(character, true);
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
#endif
}

Command* PasteCommand::Clone() {
	return new PasteCommand(*this);
}

AppID PasteCommand::GetID() {
	return AppID::ID_COMMAND_PASTE;
}

bool PasteCommand::IsUndoable() {
	return true;
}