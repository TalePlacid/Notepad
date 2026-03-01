#include <afxwin.h>
#include "SelectDocRearAction.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

SelectDocRearAction::SelectDocRearAction(CWnd* parent)
	:Action(parent) {

}

SelectDocRearAction::~SelectDocRearAction() {

}

void SelectDocRearAction::Perform() {
	//1. 페이징 버퍼에서 선택시작위치를 읽는다.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();

	//2. 선택시작위치가 없으면, 선택 표시한다.
	if (selectionBeginOffset < 0)
	{
		pagingBuffer->MarkSelectionBegin();
	}

	//3. 마지막 페이지를 적재한다.
	PageLoader::LoadLast(this->parent);
}
