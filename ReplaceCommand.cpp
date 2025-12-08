#include <afxwin.h>
#include <afxdlgs.h>
#include "ReplaceCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ByteChecker.h"
#include "MarkingHelper.h"
#include "SearchResultController.h"
#include "resource.h"
#include "PagingNavigator.h"
#include "CommandFactory.h"
#include "RowCounter.h"

#pragma warning(disable:4996)

ReplaceCommand::ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:Command(parent), source(""), replaced("") {
	this->findReplaceDialog = findReplaceDialog;
	this->offset = -1;
}

ReplaceCommand::~ReplaceCommand() {

}
 
ReplaceCommand::ReplaceCommand(const ReplaceCommand& source)
	:Command(parent) {
	this->findReplaceDialog = source.findReplaceDialog;
	this->offset = source.offset;
	this->source = source.source;
	this->replaced = source.replaced;
}

ReplaceCommand& ReplaceCommand::operator=(const ReplaceCommand& source) {
	Command::operator=(source);

	this->findReplaceDialog = source.findReplaceDialog;
	this->offset = source.offset;
	this->source = source.source;
	this->replaced = source.replaced;

	return *this;
}

void ReplaceCommand::Execute() {
	//1. 아직 변경되지 않았으면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;

	this->source = CString(searchResultController->GetKey().c_str());
	this->replaced = this->findReplaceDialog->GetReplaceString();
	if (pagingBuffer->MakeSelectedString() == this->source)
	{
		//1.1. 검색한 위치로 이동한다.
		PagingNavigator pagingNavigator(this->parent);

		Long currentIndex = searchResultController->GetCurrent();
		this->offset = searchResultController->GetAt(currentIndex).GetOffset();
		pagingNavigator.MoveTo(this->offset);

		//1.2. 노트에서 교체한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		ByteChecker byteChecker;
		Long sourceCount = RowCounter::CountCharacters(this->source);
		Long replacedCount = RowCounter::CountCharacters(this->replaced);
		TCHAR letter[2];
		GlyphFactory glyphFactory;
		Glyph* glyph;
		Long k = columnIndex;
		Long j = 0;
		Long i = 0;
		if (sourceCount < replacedCount) //원본 문자열이 더 짧으면,
		{
			//원본 문자열 길이만큼 교체한다.
			while (i < sourceCount)
			{
				letter[0] = this->replaced.GetAt(j);
				if (byteChecker.IsLeadByte(letter[0]))
				{
					letter[1] = this->replaced.GetAt(++j);
				}
				glyph = glyphFactory.Create(letter);

				row->Replace(k, glyph);
				k++;
				j++;
				i++;
			}

			//남은 길이만큼 추가한다.
			while (i < replacedCount)
			{
				letter[0] = this->replaced.GetAt(j);
				if (byteChecker.IsLeadByte(letter[0]))
				{
					letter[1] = this->replaced.GetAt(++j);
				}
				glyph = glyphFactory.Create(letter);

				row->Add(k, glyph);
				k++;
				j++;
				i++;
			}
		}
		else // 교체 문자열이 더 짧거나 같으면,
		{
			//교체 문자열 길이만큼 교체한다.
			while (i < replacedCount)
			{
				letter[0] = this->replaced.GetAt(j);
				if (byteChecker.IsLeadByte(letter[0]))
				{
					letter[1] = this->replaced.GetAt(++j);
				}
				glyph = glyphFactory.Create(letter);

				row->Replace(k, glyph);
				k++;
				j++;
				i++;
			}

			//남은 길이만큼 삭제한다.
			j = 1;
			while (j <= sourceCount - replacedCount && row->GetLength() > k)
			{
				row->Remove(k);
				j++;
			}
		}
		row->Move(k);

		//1.3. 페이징 버퍼에서 교체한다.
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		if (this->source.GetLength() < this->replaced.GetLength())
		{
			pagingBuffer->Replace(currentOffset, this->replaced.Left(this->source.GetLength()));
			pagingBuffer->Add(this->replaced.Right(this->replaced.GetLength() - this->source.GetLength()));
		}
		else
		{
			pagingBuffer->Replace(currentOffset, this->replaced);
			pagingBuffer->Remove(currentOffset + this->source.GetLength());
		}

		//1.4. 이후 검색결과들에 편차를 반영한다.
		Long difference = this->replaced.GetLength() - this->source.GetLength();
		SearchResult searchResult;
		i = searchResultController->GetCurrent() + 1;
		while (i < searchResultController->GetLength())
		{
			searchResult = searchResultController->GetAt(i);
			searchResultController->Replace(i, searchResult.OffsetBy(difference));
			i++;
		}

		//1.5. 다음 검색결과로 이동한다.
		MarkingHelper markingHelper(this->parent);
		markingHelper.Unmark();
		
		Long previousIndex = currentIndex;
		currentIndex = searchResultController->Next();
		if (previousIndex != currentIndex)
		{
			pagingNavigator.MoveTo(searchResultController->GetAt(currentIndex).GetOffset());
			rowIndex = note->GetCurrent();
			row = note->GetAt(rowIndex);
			columnIndex = row->GetCurrent();

			markingHelper.Mark();
			Long count = RowCounter::CountCharacters(this->source);
			i = 1;
			while (i <= count)
			{
				row->GetAt(columnIndex)->Select(true);
				columnIndex = row->Next();
				pagingBuffer->Next();
				i++;
			}
		}
	}
}

void ReplaceCommand::Undo() {
	//1. 위치로 이동한다.
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 선택여부를 갱신한다.
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();
	note->Select(false);

	markingHelper.Mark();

	//3. 노트에서 교체한다.
	ByteChecker byteChecker;
	GlyphFactory glyphFactory;
	Glyph* glyph;
	Long sourceCount = RowCounter::CountCharacters(this->source);
	Long replacedCount = RowCounter::CountCharacters(this->replaced);
	TCHAR letter[2];
	Long k = columnIndex;
	Long j = 0;
	Long i = 0;
	if (sourceCount <= replacedCount)
	{
		while (i < sourceCount)
		{
			letter[0] = this->source.GetAt(j);
			if (byteChecker.IsLeadByte(letter[0]))
			{
				letter[1] = this->source.GetAt(++j);
			}
			GlyphFactory glyphFactory;
			glyph = glyphFactory.Create(letter);

			row->Replace(k, glyph);
			row->GetAt(k)->Select(true);
			k++;
			j++;
			i++;
		}

		j = 1;
		while (j <= replacedCount - sourceCount && row->GetLength() > k)
		{
			row->Remove(k);
			j++;
		}
	}
	else
	{
		while (i < replacedCount)
		{
			letter[0] = this->source.GetAt(j);
			if (byteChecker.IsLeadByte(letter[0]))
			{
				letter[1] = this->source.GetAt(++j);
			}
			glyph = glyphFactory.Create(letter);

			row->Replace(k, glyph);
			row->GetAt(k)->Select(true);
			k++;
			j++;
			i++;
		}

		while (i < sourceCount)
		{
			letter[0] = this->source.GetAt(j);
			if (byteChecker.IsLeadByte(letter[0]))
			{
				letter[1] = this->source.GetAt(++j);
			}
			glyph = glyphFactory.Create(letter);

			row->Add(k, glyph);
			row->GetAt(k)->Select(true);
			k++;
			j++;
			i++;
		}
	}
	row->Move(k);

	//4. 페이징버퍼에서 교체한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	if (this->source.GetLength() <= this->replaced.GetLength())
	{
		pagingBuffer->Replace(currentOffset, this->source);
		pagingBuffer->Remove(currentOffset + this->replaced.GetLength());
	}
	else
	{
		pagingBuffer->Replace(currentOffset, this->source.Left(this->source.GetLength()));
		pagingBuffer->Add(this->source.Right(this->replaced.GetLength() - this->source.GetLength()));
	}

	//5. 이후 검색결과들에 편차를 반영한다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	Long difference = this->source.GetLength() - this->replaced.GetLength();
	SearchResult searchResult;
	i = searchResultController->GetCurrent() + 1;
	while (i < searchResultController->GetLength())
	{
		searchResult = searchResultController->GetAt(i);
		searchResultController->Replace(i, searchResult.OffsetBy(difference));
		i++;
	}
}

void ReplaceCommand::Redo() {
	//1. 검색한 위치로 이동한다.
	PagingNavigator pagingNavigator(this->parent);
	pagingNavigator.MoveTo(this->offset);

	//2. 노트에서 교체한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	ByteChecker byteChecker;
	Long sourceCount = RowCounter::CountCharacters(this->source);
	Long replacedCount = RowCounter::CountCharacters(this->replaced);
	TCHAR letter[2];
	GlyphFactory glyphFactory;
	Glyph* glyph;
	Long k = columnIndex;
	Long j = 0;
	Long i = 0;
	if (sourceCount < replacedCount) //원본 문자열이 더 짧으면,
	{
		//원본 문자열 길이만큼 교체한다.
		while (i < sourceCount)
		{
			letter[0] = this->replaced.GetAt(j);
			if (byteChecker.IsLeadByte(letter[0]))
			{
				letter[1] = this->replaced.GetAt(++j);
			}
			glyph = glyphFactory.Create(letter);

			row->Replace(k, glyph);
			k++;
			j++;
			i++;
		}

		//남은 길이만큼 추가한다.
		while (i < replacedCount)
		{
			letter[0] = this->replaced.GetAt(j);
			if (byteChecker.IsLeadByte(letter[0]))
			{
				letter[1] = this->replaced.GetAt(++j);
			}
			glyph = glyphFactory.Create(letter);

			row->Add(k, glyph);
			k++;
			j++;
			i++;
		}
	}
	else // 교체 문자열이 더 짧거나 같으면,
	{
		//교체 문자열 길이만큼 교체한다.
		while (i < replacedCount)
		{
			letter[0] = this->replaced.GetAt(j);
			if (byteChecker.IsLeadByte(letter[0]))
			{
				letter[1] = this->replaced.GetAt(++j);
			}
			glyph = glyphFactory.Create(letter);

			row->Replace(k, glyph);
			k++;
			j++;
			i++;
		}

		//남은 길이만큼 삭제한다.
		j = 1;
		while (j <= sourceCount - replacedCount && row->GetLength() > k)
		{
			row->Remove(k);
			j++;
		}
	}
	row->Move(k);

	//3. 페이징 버퍼에서 교체한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	if (this->source.GetLength() < this->replaced.GetLength())
	{
		pagingBuffer->Replace(currentOffset, this->replaced.Left(this->source.GetLength()));
		pagingBuffer->Add(this->replaced.Right(this->replaced.GetLength() - this->source.GetLength()));
	}
	else
	{
		pagingBuffer->Replace(currentOffset, this->replaced);
		pagingBuffer->Remove(currentOffset + this->source.GetLength());
	}

	//4. 이후 검색결과들에 편차를 반영한다.
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	Long difference = this->replaced.GetLength() - this->source.GetLength();
	SearchResult searchResult;
	i = searchResultController->GetCurrent() + 1;
	while (i < searchResultController->GetLength())
	{
		searchResult = searchResultController->GetAt(i);
		searchResultController->Replace(i, searchResult.OffsetBy(difference));
		i++;
	}
}

UINT ReplaceCommand::GetId() {
	return ID_COMMAND_REPLACE;
}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}

bool ReplaceCommand::IsUndoable() {
	bool ret = false;
	if (this->source != "")
	{
		ret = true;
	}
	return ret;
}

void ReplaceCommand::Update(Command* command, bool isDone) {
	Long difference;
	if (isDone)
	{
		difference = this->replaced.GetLength() - this->source.GetLength();
	}
	else
	{
		difference = this->source.GetLength() - this->replaced.GetLength();
	}

	if (!command->IsMacroCommand())
	{
		if (this->offset > command->GetOffset())
		{
			this->offset += difference;
		}
	}
	else
	{
		Long i = 0;
		while (i < command->GetLength() && command->GetAt(i)->GetOffset() < this->offset)
		{
			this->offset += difference;
			i++;
		}
	}
}

Long ReplaceCommand::GetOffset() {
	return this->offset;
}

CString ReplaceCommand::GetSource() {
	return this->source;
}

CString ReplaceCommand::GetReplaced() {
	return this->replaced;
}