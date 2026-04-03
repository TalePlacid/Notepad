#include <afxwin.h>
#include "MoveSelectionRearAction.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../CaretNavigator.h"

#pragma warning(disable:4996)

MoveSelectionRearAction::MoveSelectionRearAction(CWnd* parent)
	:Action(parent) {

}

MoveSelectionRearAction::~MoveSelectionRearAction() {

}

void MoveSelectionRearAction::Perform() {
	//1. 현재 위치가 선택 시작 위치보다 작으면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long currentOffset = pagingBuffer->GetCurrentOffset();
	Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
	if (currentOffset < selectionBeginOffset)	
	{
		//1.1. 선택 시작 위치로 이동한다.
		CaretNavigator caretNavigator(this->parent);
		caretNavigator.MoveTo(selectionBeginOffset);
	}
}