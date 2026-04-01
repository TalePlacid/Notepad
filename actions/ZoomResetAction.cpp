#include <afxwin.h>
#include "ZoomResetAction.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"
#include "../ScrollController.h"
#include "../SuspendAutoWrap.h"

#pragma warning(disable:4996)

ZoomResetAction::ZoomResetAction(CWnd* parent)
	:Action(parent) {

}

ZoomResetAction::~ZoomResetAction() {

}

void ZoomResetAction::Perform() {
	SuspendAutoWrap suspendAutoWrap(this->parent);

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	Long oldRowHeight = 0;
	Long oldVScrollMax = 0;
	if (notepadForm->sizeCalculator != NULL)
	{
		oldRowHeight = notepadForm->sizeCalculator->GetRowHeight();
	}
	if (notepadForm->scrollController != NULL)
	{
		oldVScrollMax = notepadForm->scrollController->GetVScroll().GetMax();
	}

	notepadForm->ChangeMagnification(1.0);

	LOGFONT logFont;
	notepadForm->GetOriginalFont()->GetLogFont(&logFont);

	if (notepadForm->GetDisplayFont() != NULL)
	{
		delete notepadForm->GetDisplayFont();
		notepadForm->ReplaceDisplayFont(NULL);
	}
	notepadForm->ReplaceDisplayFont(new CFont);
	notepadForm->GetDisplayFont()->CreateFontIndirect(&logFont);

	if (notepadForm->sizeCalculator != NULL)
	{
		delete notepadForm->sizeCalculator;
		notepadForm->sizeCalculator = NULL;
	}
	notepadForm->sizeCalculator = new SizeCalculator(this->parent);

	if (notepadForm->scrollController != NULL && notepadForm->scrollController->HasVScroll())
	{
		Long newRowHeight = notepadForm->sizeCalculator->GetRowHeight();
		if (oldRowHeight > 0 && newRowHeight > 0)
		{
			Long rowCount = oldVScrollMax / oldRowHeight;
			Long newVScrollMax = rowCount * newRowHeight;
			notepadForm->scrollController->ResizeVRange(newVScrollMax);
		}
	}
}



