#include <afxwin.h>
#include "ReplaceCommand.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../SearchResultController.h"
#include "../glyphs/Glyph.h"
#include "../Editor.h"

#pragma warning(disable:4996)

ReplaceCommand::ReplaceCommand(CWnd* parent, FindReplaceOption findReplaceOption)
	:Command(parent), source(""), replaced("") {
	this->findReplaceOption = findReplaceOption;
	this->columnIndex = 0;
	this->offset = -1;
}

ReplaceCommand::~ReplaceCommand() {

}

ReplaceCommand::ReplaceCommand(const ReplaceCommand& source)
	:Command(source) {
	this->findReplaceOption = source.findReplaceOption;
	this->source = source.source;
	this->replaced = source.replaced;
	this->columnIndex = source.columnIndex;
	this->offset = source.offset;
}

ReplaceCommand& ReplaceCommand::operator=(const ReplaceCommand& source) {
	Command::operator=(source);

	this->findReplaceOption = source.findReplaceOption;
	this->source = source.source;
	this->replaced = source.replaced;
	this->columnIndex = source.columnIndex;
	this->offset = source.offset;

	return *this;
}

void ReplaceCommand::Execute() {
	//1. 검색조건이 다르면, 다시 검색한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
	Editor editor(this->parent);
	if (this->findReplaceOption != searchResultController->GetFindReplaceOption())
	{
		editor.Find(this->findReplaceOption);
	}
	else if (pagingBuffer->GetSelectionBeginOffset() < 0) //2. 선택범위가 없으면, 다음으로 이동한다.
	{
		editor.FindNext();
	}
	else //3. 모두 아니면,
	{
		//3.1. 상태를 적는다.
		this->offset = pagingBuffer->GetSelectionBeginOffset();
		this->source = pagingBuffer->MakeSelectedString();
		this->replaced = this->findReplaceOption.replaceString;

		//3.2. 선택을 취소한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		note->Select(false);
		pagingBuffer->UnmarkSelectionBegin();

		//3.3. 바꾼다.
		editor.Replace(this->offset, this->source, this->replaced, TRUE, this->columnIndex);

		//3.4. 다시 찾는다.
		editor.Find(this->findReplaceOption);
#if 0
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
#endif
	}
}

void ReplaceCommand::Undo() {

}

void ReplaceCommand::Redo() {

}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}

AppID ReplaceCommand::GetID() {
	return AppID::ID_COMMAND_REPLACE;
}