#include <afxwin.h>
#include "BackspaceAction.h"
#include "../resource.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"

#pragma warning(disable:4996)

BackspaceAction::BackspaceAction(CWnd* parent)
	:KeyAction(parent) {

}

BackspaceAction::~BackspaceAction() {
	
}

void BackspaceAction::Perform() {
	if (!((NotepadForm*)(this->parent))->IsCompositing())
	{
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		if (pagingBuffer->GetSelectionBeginOffset() < 0)
		{
			Glyph* note = ((NotepadForm*)(this->parent))->note;
			Long rowIndex = note->GetCurrent();
			Glyph* row = note->GetAt(rowIndex);
			Long columnIndex = row->GetCurrent();

			if (rowIndex > 0 || columnIndex > 0)
			{
				SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_ERASE, 0);
			}
		}
		else
		{
			SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_ERASERANGE, 0);
		}
	}	
}