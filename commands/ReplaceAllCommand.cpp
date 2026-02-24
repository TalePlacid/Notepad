#include <afxwin.h>
#include "ReplaceAllCommand.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../SearchResultController.h"
#include "../Editor.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

ReplaceAllCommand::ReplaceAllCommand(CWnd* parent, FindReplaceOption findReplaceOption)
	:Command(parent), source(""), replaced("") {
	this->findReplaceOption = findReplaceOption;
	this->columnIndex = 0;
	this->offsets = 0;
	this->length = 0;
}

ReplaceAllCommand::~ReplaceAllCommand() {
	if (this->offsets != 0)
	{
		delete[] this->offsets;
	}
}

ReplaceAllCommand::ReplaceAllCommand(const ReplaceAllCommand& source)
	:Command(source) {
	this->findReplaceOption = source.findReplaceOption;
	this->source = source.source;
	this->replaced = source.replaced;
	this->columnIndex = source.columnIndex;

	if (this->offsets != 0)
	{
		delete[] this->offsets;
	}
	this->offsets = new Long[source.length];
	Long i = 0;
	while (i < source.length)
	{
		this->offsets[i] = source.offsets[i];
		i++;
	}
	this->length = source.length;
}

ReplaceAllCommand& ReplaceAllCommand::operator=(const ReplaceAllCommand& source) {
	Command::operator=(source);

	this->findReplaceOption = source.findReplaceOption;
	this->source = source.source;
	this->replaced = source.replaced;
	this->columnIndex = source.columnIndex;

	if (this->offsets != 0)
	{
		delete[] this->offsets;
	}
	this->offsets = new Long[source.length];
	Long i = 0;
	while (i < source.length)
	{
		this->offsets[i] = source.offsets[i];
		i++;
	}
	this->length = source.length;

	return *this;
}

void ReplaceAllCommand::Execute() {
	//1. 전체 문서의 첫 위치로 이동한다.
	PageLoader::LoadFirst(this->parent);

	//2. 찾는다.
	Editor editor(this->parent);
	Long count = editor.Find(this->findReplaceOption);

	//3. 검색결과가 있다면,
	if (count > 0)
	{
		//3.1. 상태를 적는다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		this->source = pagingBuffer->MakeSelectedString();
		this->replaced = this->findReplaceOption.replaceString;

		SearchResultController* searchResultController = ((NotepadForm*)(this->parent))->searchResultController;
		this->length = searchResultController->GetLength();
		this->offsets = new Long[length];
		Long i = 0;
		while (i < length)
		{
			this->offsets[i] = searchResultController->GetAt(i);
			i++;
		}

		//3.2. 선택을 취소한다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		note->Select(false);
		pagingBuffer->UnmarkSelectionBegin();

		//3.3. 끝까지 바꾼다.
		i = 0;
		editor.Replace(this->offsets[i], this->source, this->replaced, TRUE, this->columnIndex);
		while (editor.FindNext())
		{
			i++;
			note->Select(false);
			pagingBuffer->UnmarkSelectionBegin();

			editor.Replace(this->offsets[i], this->source, this->replaced, TRUE, this->columnIndex);
		}
	}
}

void ReplaceAllCommand::Undo() {

}

void ReplaceAllCommand::Redo() {

}

Command* ReplaceAllCommand::Clone() {
	return new ReplaceAllCommand(*this);
}

AppID ReplaceAllCommand::GetID() {
	return  AppID::ID_COMMAND_REPLACE_ALL;
}