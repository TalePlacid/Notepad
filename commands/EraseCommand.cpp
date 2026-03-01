#include "EraseCommand.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../ScrollController.h"
#include "../SizeCalculator.h"
#include "../glyphs/GlyphFactory.h"
#include "../CaretNavigator.h"
#include "../NoteWrapper.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

EraseCommand::EraseCommand(CWnd* parent, BOOL onChar)
	:Command(parent) {
	this->onChar = onChar;
	this->offset = -1;
}

EraseCommand::~EraseCommand() {

}

EraseCommand::EraseCommand(const EraseCommand& source)
	:Command(source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;
}

EraseCommand& EraseCommand::operator=(const EraseCommand& source) {
	Command::operator=(source);

	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->onChar = source.onChar;
	this->offset = source.offset;

	return *this;
}

void EraseCommand::Execute() {
	//1. 파일의 시작 위치가 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetCurrentOffset() > 0
		|| (!this->onChar && ((NotepadForm*)(this->parent))->IsCompositing()))
	{
		//1.1. 현재 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//1.2. 줄의 처음이 아니라면,
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long vScrollChanged = 0;
		if (columnIndex > 0)
		{
			//1.2.1. 노트에서 지운다.
			Glyph* letter = row->GetAt(columnIndex - 1);
			this->character[0] = ((char*)*letter)[0];
			if (letter->IsMultiByteCharacter())
			{
				this->character[1] = ((char*)*letter)[1];
			}

			row->Remove(columnIndex - 1);
			columnIndex = row->GetCurrent();

			//1.2.2. 자동개행중이라면,
			if (((NotepadForm*)(this->parent))->IsAutoWrapped())
			{
				//1.2.2.1. 재개행한다.
				NoteWrapper noteWrapper(this->parent);
				vScrollChanged = noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
		}
		else //1.3. 줄의 처음이면,
		{
			//1.3.1. 적재범위를 넘어서면 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				PageLoader::LoadPrevious(this->parent);
				rowIndex = note->GetCurrent();
			}

			//1.3.2. 첫번째 줄이 아니면,
			if (rowIndex > 0)
			{
				this->character[0] = '\r';
				this->character[1] = '\n';

				//1.3.2.1. 줄을 합친다.
				Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex - 1);
				columnIndex += previousRow->GetLength();
				note->MergeRows(rowIndex - 1);

				rowIndex = note->Previous();
				row = note->GetAt(rowIndex);
				row->Move(columnIndex);

				vScrollChanged = -1;
			}
		}

		//1.4. 페이징버퍼에서 지운다.
		if (this->onChar)
		{
			pagingBuffer->Remove();
			this->isUndoable = true;
		}

		//1.5. 스크롤이 있으면, 적용한다.
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}

		this->offset = pagingBuffer->GetCurrentOffset();
		this->columnIndex = columnIndex;
	}
 }

void EraseCommand::Undo() {
	//1. 오프셋이 존재하면,
	if (this->offset >= 0)
	{
		//1.1. 오프셋 기반으로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(this->offset);
		caretNavigator.NormalizeColumn(this->columnIndex);

		//1.2. 현재 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//1.3. 줄 바꿈 문자가 아니면,
		Long vScrollChanged = 0;
		if (this->character[0] != '\r')
		{
			//1.3.1. 노트에서 쓴다.
			GlyphFactory glyphFactory;
			Glyph* glyph = glyphFactory.Create(this->character);
			row->Add(columnIndex, glyph);
			columnIndex = row->GetCurrent();

			//1.3.2. 자동개행중이면, 재개행한다.
			if (((NotepadForm*)(this->parent))->IsAutoWrapped())
			{
				NoteWrapper noteWrapper(this->parent);
				vScrollChanged = noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
		}
		else //1.4. 줄바꿈 문자이면,
		{
			//1.4.1. 줄을 나눈다.
			note->SplitRows(rowIndex, columnIndex);
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();

			vScrollChanged = 1;
		}

		//1.5. 페이징 버퍼에서 쓴다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		pagingBuffer->Add(this->character);
	
		//1.6. 수직 스크롤이 있으면, 반영한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}

		this->offset = pagingBuffer->GetCurrentOffset();
		this->columnIndex = columnIndex;
	}
}

void EraseCommand::Redo() {
	//1. 오프셋이 존재하면,
	if (this->offset >= 0) 
	{
		//1.1. 오프셋 기반으로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(this->offset);
		caretNavigator.NormalizeColumn(this->columnIndex);

		//1.2. 현재 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//1.3. 줄의 처음이 아니면,
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long vScrollChanged = 0;
		if (columnIndex > 0)
		{
			//1.3.1. 노트에서 지운다.
			row->Remove(columnIndex - 1);
			columnIndex = row->GetCurrent();

			//1.3.2. 자동개행중이라면,
			if (((NotepadForm*)(this->parent))->IsAutoWrapped())
			{
				NoteWrapper noteWrapper(this->parent);
				vScrollChanged = noteWrapper.Rewrap();
				rowIndex = note->GetCurrent();
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
		}
		else //1.4. 줄의 처음이면,
		{
			//1.4.1. 적재 범위를 넘어섰으면, 재적재한다.
			if (note->IsAboveTopLine(rowIndex - 1) && pagingBuffer->GetRowStartIndex() > 0)
			{
				PageLoader::LoadPrevious(this->parent);
				rowIndex = note->GetCurrent();
			}

			//1.4.2. 첫번째 줄이 아니면,
			if (rowIndex > 0)
			{
				//1.4.2.1. 이전 줄이랑 합친다.
				Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex - 1);
				columnIndex += previousRow->GetLength();

				rowIndex = note->MergeRows(rowIndex - 1);
				rowIndex = note->Previous();
				row = note->GetAt(rowIndex);
				columnIndex = row->Move(columnIndex);

				vScrollChanged = -1;
			}
		}

		//1.5. 페이징버퍼에서 지운다.
		pagingBuffer->Remove();
	
		//1.6. 수직 스크롤이 있다면, 반영한다.
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		if (scrollController->HasVScroll())
		{
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax() + vScrollChanged * sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
		}

		this->offset = pagingBuffer->GetCurrentOffset();
		this->columnIndex = columnIndex;
	}
}

Command* EraseCommand::Clone() {
	return new EraseCommand(*this);
}

