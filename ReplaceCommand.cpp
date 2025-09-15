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
	//1. ������� �ƴϸ�, ����Ѵ�.
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
	else //2. ������̸�, �ش� ��ġ�� �̵��Ѵ�.
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

	//3. ��Ʈ���� ��ü�Ѵ�.
	ByteChecker byteChecker;
	Long sourceCount = byteChecker.CountCharacters(this->source);
	Long replacedCount = byteChecker.CountCharacters(this->replaced);
	TCHAR letter[2];
	GlyphFactory glyphFactory;
	Glyph* glyph;
	Long k = columnIndex;
	Long j = 0;
	Long i = 0;
	if (sourceCount < replacedCount) //���� ���ڿ��� �� ª����,
	{
		//���� ���ڿ� ���̸�ŭ ��ü�Ѵ�.
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

		//���� ���̸�ŭ �߰��Ѵ�.
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
	else // ��ü ���ڿ��� �� ª�ų� ������,
	{
		//��ü ���ڿ� ���̸�ŭ ��ü�Ѵ�.
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
		
		//���� ���̸�ŭ �����Ѵ�.
		j = 1;
		while (j <= sourceCount - replacedCount && row->GetLength() > k)
		{
			row->Remove(k);
			j++;
		}
	}
	row->Move(k);

	//4. ����¡ ���ۿ��� ��ü�Ѵ�.
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

	//5. ���� �˻�����鿡 ������ �ݿ��Ѵ�.
	Long difference = this->replaced.GetLength() - this->source.GetLength();
	SearchResult searchResult;
	i = searchResultController->GetCurrent() + 1;
	while (i < searchResultController->GetLength())
	{
		searchResult = searchResultController->GetAt(i);
		searchResultController->Replace(i, searchResult.OffsetBy(difference));
		i++;
	}
	
	//6. ������ ����Ѵ�.
	MarkingHelper markingHelper(this->parent);
	markingHelper.Unmark();
	note->Select(false);

	//7. Ŭ���̾�Ʈ ������ �����Ѵ�.
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