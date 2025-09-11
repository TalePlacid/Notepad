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

#pragma warning(disable:4996)

ReplaceCommand::ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:FindReplaceCommand(parent, findReplaceDialog) {

}

ReplaceCommand::~ReplaceCommand() {

}
 
void ReplaceCommand::Execute() {
	//1. 노트에서 교체한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long markedRowIndex = note->GetMarked();
	markedRowIndex = note->Move(markedRowIndex);

	Glyph* row = note->GetAt(markedRowIndex);
	Long markedColumnIndex = row->GetMarked();
	markedColumnIndex = row->Move(markedColumnIndex);

	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	CString replaceString(this->findReplaceDialog->GetReplaceString());

	Glyph* character;
	Glyph* glyph;
	GlyphFactory glyphFactory;
	ByteChecker byteChecker;
	TCHAR contents[2];
	BOOL isSelected = TRUE;
	Long j = markedColumnIndex;
	Long i = 0;
	while (i < replaceString.GetLength() && j < row->GetLength())
	{
		character = row->GetAt(j);
		if (character->IsSelected())
		{
			contents[0] = replaceString.GetAt(i);
			if (byteChecker.IsLeadByte(contents[0]))
			{
				contents[1] = replaceString.GetAt(++i);
			}

			glyph = glyphFactory.Create(contents);

			row->Replace(j, glyph);
			j = row->Next();
		}

		i++;
	}

	while (i < replaceString.GetLength())
	{
		contents[0] = replaceString.GetAt(i);
		if (byteChecker.IsLeadByte(contents[0]))
		{
			contents[1] = replaceString.GetAt(++i);
		}

		glyph = glyphFactory.Create(contents);

		row->Add(j, glyph);

		i++;
	}

	Long currentIndex = row->GetCurrent();
	character = row->GetAt(currentIndex);
	while (row->GetLength() > currentIndex && character->IsSelected())
	{
		row->Remove(currentIndex);
		character = row->GetAt(currentIndex);
	}

	//2. 페이징 버퍼에서 교체한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	CString findString(searchResultController->GetKey().c_str());
	Long currentSearchIndex = searchResultController->GetCurrent();
	Long replaceStartOffset = searchResultController->GetAt(currentSearchIndex).GetOffset();

	if (replaceString.GetLength() < findString.GetLength())
	{
		pagingBuffer->Replace(replaceStartOffset, replaceString);
		pagingBuffer->Remove(replaceStartOffset + findString.GetLength());
	}
	else
	{
		pagingBuffer->Replace(replaceStartOffset, replaceString.Left(findString.GetLength()));
		pagingBuffer->Add(replaceString.Right(replaceString.GetLength() - findString.GetLength()));
	}

	//3. 이후 검색결과들에 길이변화를 반영한다.
	Long difference = replaceString.GetLength() - findString.GetLength();
	SearchResult searchResult;
	i = currentSearchIndex + 1;
	while (i < searchResultController->GetLength())
	{
		searchResult = searchResultController->GetAt(i);
		searchResultController->Replace(i, searchResult.OffsetBy(difference));
		i++;
	}

	//4. 선택을 취소한다.
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();
	note->Select(false);

	//5. 클라이언트 영역을 갱신한다.
	((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
	this->parent->Invalidate();
}

void ReplaceCommand::Undo() {

}

UINT ReplaceCommand::GetId() {
	return 0;
}

Command* ReplaceCommand::Clone() {
	return 0;
}