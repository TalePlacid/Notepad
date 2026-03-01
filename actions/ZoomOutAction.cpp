#include <afxwin.h>
#include "ZoomOutAction.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

ZoomOutAction::ZoomOutAction(CWnd* parent)
	:Action(parent) {

}

ZoomOutAction::~ZoomOutAction() {

}

void ZoomOutAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	notepadForm->ChangeMagnification(notepadForm->GetMagnification() - 0.1);
	if (notepadForm->GetMagnification() < 0.1)
	{
		notepadForm->ChangeMagnification(0.1);
	}

	LOGFONT logFont;
	notepadForm->GetOriginalFont()->GetLogFont(&logFont);

	double round = 0.5;
	if (logFont.lfHeight < 0)
	{
		round = -0.5;
	}
	logFont.lfHeight = logFont.lfHeight * notepadForm->GetMagnification() + round;
	if (notepadForm->GetDisplayFont() != NULL)
	{
		delete notepadForm->GetDisplayFont();
		notepadForm->ReplaceDisplayFont(NULL);
	}
	notepadForm->ReplaceDisplayFont(new CFont);
	notepadForm->GetDisplayFont()->CreateFontIndirectA(&logFont);

	if (notepadForm->sizeCalculator != NULL)
	{
		delete notepadForm->sizeCalculator;
		notepadForm->sizeCalculator = NULL;
	}
	notepadForm->sizeCalculator = new SizeCalculator(this->parent);
}


