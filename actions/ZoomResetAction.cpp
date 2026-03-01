#include <afxwin.h>
#include "ZoomResetAction.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

ZoomResetAction::ZoomResetAction(CWnd* parent)
	:Action(parent) {

}

ZoomResetAction::~ZoomResetAction() {

}

void ZoomResetAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	notepadForm->ChangeMagnification(1.0);

	LOGFONT logFont;
	notepadForm->GetOriginalFont()->GetLogFont(&logFont);

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


