#include <afxwin.h>
#include <afxdlgs.h>
#include "ReplaceCommand.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ByteChecker.h"
#include "SearchResultController.h"
#include "resource.h"
#include "CaretNavigator.h"
#include "CommandFactory.h"
#include "RowCounter.h"
#include "NoteWrapper.h"
#include "FindCommand.h"

#pragma warning(disable:4996)

ReplaceCommand::ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:Command(parent){
	this->findReplaceDialog = findReplaceDialog;

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	this->findReplaceOption = searchResultController->GetFindReplaceOption();

	this->source = pagingBuffer->MakeSelectedString();
	this->replaced = this->findReplaceOption.replaceString;
	this->index = searchResultController->GetCurrent();
	this->offset = searchResultController->GetAt(this->index);
	this->unexecuted = TRUE;
}

ReplaceCommand::~ReplaceCommand() {

}
 
ReplaceCommand::ReplaceCommand(const ReplaceCommand& source)
	:Command(parent), findReplaceOption(source.findReplaceOption) {
	this->findReplaceDialog = source.findReplaceDialog;
	this->index = source.index;
	this->offset = source.offset;
	this->source = source.source;
	this->replaced = source.replaced;
	this->unexecuted = source.unexecuted;
}

ReplaceCommand& ReplaceCommand::operator=(const ReplaceCommand& source) {
	Command::operator=(source);

	this->findReplaceDialog = source.findReplaceDialog;
	this->index = source.index;
	this->offset = source.offset;
	this->source = source.source;
	this->replaced = source.replaced;
	this->findReplaceOption = source.findReplaceOption;
	this->unexecuted = source.unexecuted;

	return *this;
}

void ReplaceCommand::Execute() {
	((NotepadForm*)(this->parent))->isDirty = TRUE;

	//1. 검색된 위치로 이동한다.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(0);
	
	//2. 선택해제한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 현재 위치를 읽는다.
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//4. 공통길이만큼 교체한다.
	Long sourceLength = RowCounter::CountCharacters(this->source);
	Long replacedLength = RowCounter::CountCharacters(this->replaced);
	Long commonLength;
	if (sourceLength < replacedLength)
	{
		commonLength = sourceLength;
	}
	else
	{
		commonLength = replacedLength;
	}

	ByteChecker byteChecker;
	TCHAR character[2];
	GlyphFactory glyphFactory;
	Glyph* glyph;
	Long j = 0;
	Long i = 0;
	while (i < commonLength)
	{
		if (columnIndex < row->GetLength())
		{
			character[0] = this->replaced.GetAt(j);
			if (byteChecker.IsLeadByte(character[0]))
			{
				character[1] = this->replaced.GetAt(++j);
			}

			glyph = glyphFactory.Create(character);
			glyph->Select(true);
			row->Replace(columnIndex, glyph);
			columnIndex = row->Next();

			j++;
		}

		if (columnIndex >= row->GetLength())
		{
			rowIndex = note->Next();
			row = note->GetAt(rowIndex);
			columnIndex = row->First();
		}

		i++;
	}

	//5. 바꾼 문자열이 더 길면, 노트에서 남는 길이만큼 추가한다.
	if (replacedLength > commonLength)
	{
		while (j < this->replaced.GetLength())
		{
			character[0] = this->replaced.GetAt(j);
			if (byteChecker.IsLeadByte(character[0]))
			{
				character[1] = this->replaced.GetAt(++j);
			}

			glyph = glyphFactory.Create(character);
			glyph->Select(true);
			row->Add(columnIndex, glyph);
			columnIndex = row->GetCurrent();

			j++;
		}
	}
	else if (sourceLength > commonLength) //6. 원본 문자열이 더 길면, 노트에서 남는 길이만큼 삭제한다.
	{
		Long restLength = sourceLength - commonLength;
		i = 0;
		while (i < restLength)
		{
			if (columnIndex < row->GetLength())
			{
				row->Remove(columnIndex);
				columnIndex = row->Move(columnIndex);
			}

			if (columnIndex >= row->GetLength())
			{
				note->MergeRows(rowIndex);
			}

			i++;
		}
	}

	//7. 자동개행중이면, 재개행한다.
	if (((NotepadForm*)(this->parent))->isAutoWrapped)
	{
		NoteWrapper noteWrapper(this->parent);
		noteWrapper.Rewrap();
	}

	//8. 페이징 버퍼에서 수정한다.
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

	//9. 실행이면, 다시 찾는다.
	if (this->unexecuted)
	{
		this->unexecuted = FALSE;
		FindCommand findCommand(this->parent, this->findReplaceDialog);
		findCommand.Execute();
	}
	else //10. 실행이 아니면, 검색옵션을 초기화한다.
	{
		SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
		searchResultController->ChangeFindReplaceOption(FindReplaceOption());
	}
}

void ReplaceCommand::Undo() {
	CString temp(this->source);
	this->source = this->replaced;
	this->replaced = temp;

	this->Execute();
}

void ReplaceCommand::Redo() {
	CString temp(this->source);
	this->source = this->replaced;
	this->replaced = temp;

	this->Execute();
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

Long ReplaceCommand::GetOffset() {
	return this->offset;
}

CString ReplaceCommand::GetSource() {
	return this->source;
}

CString ReplaceCommand::GetReplaced() {
	return this->replaced;
}