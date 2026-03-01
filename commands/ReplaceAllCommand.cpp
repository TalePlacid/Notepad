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
		this->isUndoable = true;

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

		//3.2. 끝까지 바꾼다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long difference = this->replaced.GetLength() - this->source.GetLength();
		Long j;
		i = 0;
		while (i < this->length)
		{
			note->Select(false);
			pagingBuffer->UnmarkSelectionBegin();
			editor.Replace(this->offsets[i], this->source, this->replaced, TRUE, this->columnIndex);

			j = i + 1;
			while (j < this->length)
			{
				this->offsets[j] += difference;
				j++;
			}

			i++;
		}

		((NotepadForm*)(this->parent))->searchResultController->ChangeFindReplaceOption(FindReplaceOption());
	}
}

void ReplaceAllCommand::Undo() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Editor editor(this->parent);

	Long difference = this->source.GetLength() - this->replaced.GetLength();
	Long j;
	Long i = 0;
	while (i < this->length)
	{
		note->Select(false);
		pagingBuffer->UnmarkSelectionBegin();
		editor.Replace(this->offsets[i], this->replaced, this->source, TRUE, this->columnIndex);

		j = i + 1;
		while (j < this->length)
		{
			this->offsets[j] += difference;
			j++;
		}

		i++;
	}
}

void ReplaceAllCommand::Redo() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Editor editor(this->parent);

	Long difference = this->replaced.GetLength() - this->source.GetLength();
	Long j;
	Long i = 0;
	while (i < this->length)
	{
		note->Select(false);
		pagingBuffer->UnmarkSelectionBegin();
		editor.Replace(this->offsets[i], this->source, this->replaced, TRUE, this->columnIndex);

		j = i + 1;
		while (j < this->length)
		{
			this->offsets[j] += difference;
			j++;
		}

		i++;
	}
}

Command* ReplaceAllCommand::Clone() {
	return new ReplaceAllCommand(*this);
}
