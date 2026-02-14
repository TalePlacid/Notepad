#include <afxwin.h>
#include "HScrollBarDragAction.h"
#include "../NotepadForm.h"
#include "../ScrollController.h"

#pragma warning(disable:4996)

HScrollBarDragAction::HScrollBarDragAction(CWnd* parent, int nPos)
	:ScrollBarAction(parent) {
	this->nPos = nPos;
}

HScrollBarDragAction::~HScrollBarDragAction() {

}

void HScrollBarDragAction::Perform() {
	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	scrollController->MoveHScroll(this->nPos);
}