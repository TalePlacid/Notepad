#include <afxwin.h>
#include "MoveSelectionFrontAction.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

MoveSelectionFrontAction::MoveSelectionFrontAction(CWnd* parent)
	:Action(parent) {

}

MoveSelectionFrontAction::~MoveSelectionFrontAction() {

}

void MoveSelectionFrontAction::Perform() {
	//1. 현재 위치가 범위 시작 위치보다 크면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	if (currentOffset > selectionBeginOffset)
	{
		//1.1. 범위 선택 위치로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(selectionBeginOffset);
	}
}