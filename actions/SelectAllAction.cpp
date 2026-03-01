#include <afxwin.h>
#include "SelectAllAction.h"
#include "../PagingBuffer.h"
#include "../NotepadForm.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

SelectAllAction::SelectAllAction(CWnd* parent)
	:Action(parent) {

}

SelectAllAction::~SelectAllAction() {

}

void SelectAllAction::Perform() {
	//1. 페이징 버퍼에서 첫 위치로 이동한다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->FirstRow();
	pagingBuffer->MarkSelectionBegin();

	//2. 마지막 페이지를 적재한다.
	PageLoader::LoadLast(this->parent);
}
