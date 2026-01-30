#include <afxwin.h>
#include "ZoomInCommand.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

ZoomInCommand::ZoomInCommand(CWnd* parent)
	:Command(parent) {

}

ZoomInCommand::~ZoomInCommand() {

}

void ZoomInCommand::Execute() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);

	notepadForm->magnification += 0.1;
	if (notepadForm->magnification > 5.0)
	{
		notepadForm->magnification = 5.0;
	}

	LOGFONT logFont;
	notepadForm->originalFont->GetLogFont(&logFont);

	double round = 0.5;
	if (logFont.lfHeight < 0)
	{
		round = -0.5;
	}
	logFont.lfHeight = logFont.lfHeight * notepadForm->magnification + round;
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