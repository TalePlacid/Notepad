#include <afxwin.h>
#include "AutoWrapCommand.h"
#include "NotepadForm.h"
#include "resource.h"
#include "NoteWrapper.h"
#include "PagingBuffer.h"
#include "ScrollController.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

AutoWrapCommand::AutoWrapCommand(CWnd* parent)
	:Command(parent) {

}

AutoWrapCommand::~AutoWrapCommand() {

}

void AutoWrapCommand::Execute() {
	SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADFIRST, 0);

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	notepadForm->isAutoWrapped = !notepadForm->isAutoWrapped;
	NoteWrapper noteWrapper(this->parent);

	ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	Long fileRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());
	Long max = fileRowCount * sizeCalculator->GetRowHeight();
	if (notepadForm->isAutoWrapped)
	{
		Long dummyRowCount = noteWrapper.Wrap();
		max += dummyRowCount * sizeCalculator->GetRowHeight();
		notepadForm->menu.CheckMenuItem(ID_MENU_AUTOWRAP, MF_CHECKED);
	}
	else
	{
		noteWrapper.Unwrap();
		notepadForm->menu.CheckMenuItem(ID_MENU_AUTOWRAP, MF_UNCHECKED);
	}

	if (scrollController->HasVScroll())
	{

		scrollController->ResizeVRange(max);
	}
}