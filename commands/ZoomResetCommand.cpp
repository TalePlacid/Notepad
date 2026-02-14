#include <afxwin.h>
#include "ZoomResetCommand.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

ZoomResetCommand::ZoomResetCommand(CWnd* parent)
	:Command(parent) {

}

ZoomResetCommand::~ZoomResetCommand() {

}

void ZoomResetCommand::Execute() {
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