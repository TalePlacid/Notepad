#include <afxwin.h>
#include "ReplaceCommand.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
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
	//1. 상태를 적는다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	this->offset = pagingBuffer->GetSelectionBeginOffset();
	this->source = pagingBuffer->MakeSelectedString();
	this->replaced = this->findReplaceOption.replaceString;

	//2. 선택을 취소한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	note->Select(false);
	pagingBuffer->UnmarkSelectionBegin();

	//3. 바꾼다.
	Editor editor(this->parent);
	editor.Replace(this->offset, this->source, this->replaced, TRUE, this->columnIndex);

	//4. 다시 찾는다.
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