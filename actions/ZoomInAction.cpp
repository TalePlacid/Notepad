#include <afxwin.h>
#include "ZoomInAction.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"
#include "../FontSelector.h"

#pragma warning(disable:4996)

ZoomInAction::ZoomInAction(CWnd* parent)
	:Action(parent) {

}

ZoomInAction::~ZoomInAction() {

}

void ZoomInAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

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
}



