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

	notepadForm->magnification = 1.0;

	LOGFONT logFont;
	notepadForm->originalFont->GetLogFont(&logFont);

	if (notepadForm->displayFont != NULL)
	{
		delete notepadForm->displayFont;
		notepadForm->displayFont = NULL;
	}
	notepadForm->displayFont = new CFont;
	notepadForm->displayFont->CreateFontIndirectA(&logFont);

	if (notepadForm->sizeCalculator != NULL)
	{
		delete notepadForm->sizeCalculator;
		notepadForm->sizeCalculator = NULL;
	}
	notepadForm->sizeCalculator = new SizeCalculator(this->parent);
}