#include <afxwin.h>
#include "ZoomInAction.h"
#include "../NotepadForm.h"
#include "../glyphs/NoteWidthCache.h"
#include "../SizeCalculator.h"
#include "../ScrollController.h"
#include "../FontSelector.h"
#include "../SuspendAutoWrap.h"

#pragma warning(disable:4996)

ZoomInAction::ZoomInAction(CWnd* parent)
	:Action(parent) {

}

ZoomInAction::~ZoomInAction() {

}

void ZoomInAction::Perform() {
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

	notepadForm->ChangeMagnification(notepadForm->GetMagnification() + 0.1);
	if (notepadForm->GetMagnification() > 5.0)
	{
		notepadForm->ChangeMagnification(5.0);
	}

	LOGFONT logFont;
	notepadForm->GetOriginalFont()->GetLogFont(&logFont);

	FontSelector fontSelector;
	logFont = fontSelector.SelectScaledLogFont(logFont, notepadForm->GetMagnification());
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
	notepadForm->noteWidthCache->MarkAllDirty();

	if (notepadForm->scrollController->HasVScroll())
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



