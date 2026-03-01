#include <afxwin.h>
#include <afxdlgs.h>
#include "ChangeFontAction.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"
#include "../ScrollBarAnalyzer.h"
#include "../ScrollController.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

ChangeFontAction::ChangeFontAction(CWnd* parent)
	:Action(parent) {

}

ChangeFontAction::~ChangeFontAction() {

}

void ChangeFontAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	LOGFONT logFont = { 0, };
	notepadForm->GetOriginalFont()->GetLogFont(&logFont);
	CFontDialog cFontDialog(&logFont);
	INT_PTR result = cFontDialog.DoModal();
	if (result == IDOK)
	{
		LOGFONT logFont;
		cFontDialog.GetCurrentFont(&logFont);

		if (notepadForm->GetOriginalFont() != NULL)
		{
			delete notepadForm->GetOriginalFont();
			notepadForm->ReplaceOriginalFont(NULL);
		}
		notepadForm->ReplaceOriginalFont(new CFont);
		notepadForm->GetOriginalFont()->CreateFontIndirectA(&logFont);

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
		SizeCalculator* sizeCalculator = notepadForm->sizeCalculator;

		ScrollBarAnalyzer scrollBarAnalyzer(this->parent);
		scrollBarAnalyzer.AnalyzeWithoutWrap();

		ScrollController* scrollController = notepadForm->scrollController;
		if (scrollBarAnalyzer.GetVScrollNeeded())
		{
			scrollController->ShowVScroll();

			PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
			Long rowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());
			Long max = rowCount * sizeCalculator->GetRowHeight();
			scrollController->ResizeVRange(max);
			scrollController->ResizeVPage(scrollBarAnalyzer.GetClientAreaHeight());
		}
		else
		{
			scrollController->ShowVScroll(false);
		}

		if (scrollBarAnalyzer.GetHScrollNeeded())
		{
			scrollController->ShowHScroll();
			scrollController->ResizeHRange(scrollBarAnalyzer.GetContentsWidth());
			scrollController->ResizeHPage(scrollBarAnalyzer.GetClientAreaWidth());
		}
		else
		{
			scrollController->ShowHScroll(false);
		}
	}
}



