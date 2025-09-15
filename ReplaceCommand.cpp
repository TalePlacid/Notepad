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

#pragma warning(disable:4996)

ReplaceCommand::ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:FindReplaceCommand(parent, findReplaceDialog), source(""), replaced("") {
	this->offset = -1;
}

ReplaceCommand::~ReplaceCommand() {

}
 
ReplaceCommand::ReplaceCommand(const ReplaceCommand& source)
	:FindReplaceCommand(parent, findReplaceDialog) {
	this->offset = source.offset;
	this->source = source.source;
	this->replaced = source.replaced;
}

ReplaceCommand& ReplaceCommand::operator=(const ReplaceCommand& source) {
	FindReplaceCommand::operator=(source);

	this->offset = source.offset;
	this->source = source.source;
	this->replaced = source.replaced;

	return *this;
}

void ReplaceCommand::Execute() {
	//1. 재실행이 아니면, 기록한다.
	Glyph* note;
	Glyph* row;
	Long rowIndex;
	Long columnIndex;
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (this->offset < 0)
	{
		Long currentIndex = searchResultController->GetCurrent();
		pagingBuffer->MoveOffset(searchResultController->GetAt(currentIndex).GetOffset());
		this->offset = pagingBuffer->GetCurrentOffset();
		this->source = CString(searchResultController->GetKey().c_str());
		this->replaced = this->findReplaceDialog->GetReplaceString();
	}
	else //2. 재실행이면, 해당 위치로 이동한다.
	{
		pagingBuffer->MoveOffset(this->offset);
		if (!pagingBuffer->IsOnPage(this->offset))
		{
			pagingBuffer->Load();
		}
	}
	note = ((NotepadForm*)(this->parent))->note;
	rowIndex = note->Move(pagingBuffer->GetCurrent().GetRow());
	row = note->GetAt(rowIndex);
	columnIndex = row->Move(pagingBuffer->GetCurrent().GetColumn());

	//3. 노트에서 교체한다.
	ByteChecker byteChecker;
	Long sourceCount = byteChecker.CountCharacters(this->source);
	Long replacedCount = byteChecker.CountCharacters(this->replaced);
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

	//4. 페이징 버퍼에서 교체한다.
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

	//5. 이후 검색결과들에 편차를 반영한다.
	Long difference = this->replaced.GetLength() - this->source.GetLength();
	SearchResult searchResult;
	i = searchResultController->GetCurrent() + 1;
	while (i < searchResultController->GetLength())
	{
		searchResult = searchResultController->GetAt(i);
		searchResultController->Replace(i, searchResult.OffsetBy(difference));
		i++;
	}
	
	//6. 선택을 취소한다.
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();
	note->Select(false);

	//7. 클라이언트 영역을 갱신한다.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}

void ReplaceCommand::Undo() {

}

UINT ReplaceCommand::GetId() {
	return ID_COMMAND_REPLACE;
}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}

bool ReplaceCommand::IsUndoable() {
	return true;
}