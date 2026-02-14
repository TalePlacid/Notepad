#include <afxdlgs.h>
#include "SelectFontCommand.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"
#include "../ScrollBarAnalyzer.h"
#include "../ScrollController.h"
#include "../PagingBuffer.h"

#pragma warning(disable:4996)

SelectFontCommand::SelectFontCommand(CWnd* parent)
	:Command(parent) {

}

SelectFontCommand::~SelectFontCommand() {

}

void SelectFontCommand::Execute() {
	CFontDialog cFontDialog;
	INT_PTR result = cFontDialog.DoModal();
	if (result == IDOK)
	{
		LOGFONT logFont;
		cFontDialog.GetCurrentFont(&logFont);

		if (((NotepadForm*)(this->parent))->originalFont != NULL)
		{
			delete ((NotepadForm*)(this->parent))->originalFont;
			((NotepadForm*)(this->parent))->originalFont = NULL;
		}
		((NotepadForm*)(this->parent))->originalFont = new CFont;
		((NotepadForm*)(this->parent))->originalFont->CreateFontIndirectA(&logFont);

		double round = 0.5;
		if (logFont.lfHeight < 0)
		{
			round = -0.5;
		}
		logFont.lfHeight = logFont.lfHeight * ((NotepadForm*)(this->parent))->magnification + round;
		if (((NotepadForm*)(this->parent))->displayFont != NULL)
		{
			delete ((NotepadForm*)(this->parent))->displayFont;
			((NotepadForm*)(this->parent))->displayFont = NULL;
		}
		((NotepadForm*)(this->parent))->displayFont = new CFont;
		((NotepadForm*)(this->parent))->displayFont->CreateFontIndirectA(&logFont);

		if (((NotepadForm*)(this->parent))->sizeCalculator != NULL)
		{
			delete ((NotepadForm*)(this->parent))->sizeCalculator;
			((NotepadForm*)(this->parent))->sizeCalculator = NULL;
		}
		((NotepadForm*)(this->parent))->sizeCalculator = new SizeCalculator(this->parent);
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

		ScrollBarAnalyzer scrollBarAnalyzer(this->parent);
		scrollBarAnalyzer.AnalyzeWithoutWrap();

		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		if (scrollBarAnalyzer.GetVScrollNeeded())
		{
			scrollController->ShowVScroll();

			PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
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