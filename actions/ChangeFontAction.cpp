#include <afxwin.h>
#include <afxdlgs.h>
#include "ChangeFontAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../SizeCalculator.h"
#include "../ScrollBarAnalyzer.h"
#include "../ScrollController.h"
#include "../PagingBuffer.h"
#include "../NoteWrapper.h"
#include "../FontSelector.h"

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
	
	DWORD flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	CFontDialog cFontDialog(&logFont, flags);
	INT_PTR result = cFontDialog.DoModal();
	if (result == IDOK)
	{
		//1. 자동개행중이라면, 자동개행을 푼다.
		NoteWrapper noteWrapper(this->parent);
		Long dummyRowCount = 0;
		if (notepadForm->IsAutoWrapped())
		{
			dummyRowCount = noteWrapper.Unwrap();
		}

		//2. 수직 스크롤바가 있다면, max를 줄인다.
		ScrollController* scrollController = notepadForm->scrollController;
		Scroll vScroll = scrollController->GetVScroll();
		Long rowHeight = notepadForm->sizeCalculator->GetRowHeight();
		if (scrollController->HasVScroll())
		{
			Long max = vScroll.GetMax() - dummyRowCount * rowHeight;
			scrollController->ResizeVRange(max);
		}

		//3. 폰트 설정을 바꾼다.
		LOGFONT logFont;
		cFontDialog.GetCurrentFont(&logFont);

		if (notepadForm->GetOriginalFont() != NULL)
		{
			delete notepadForm->GetOriginalFont();
			notepadForm->ReplaceOriginalFont(NULL);
		}
		notepadForm->ReplaceOriginalFont(new CFont);
		notepadForm->GetOriginalFont()->CreateFontIndirect(&logFont);

		FontSelector fontSelector;
		logFont = fontSelector.SelectScaledLogFont(logFont, notepadForm->GetMagnification());
		if (notepadForm->GetDisplayFont() != NULL)
		{
			delete notepadForm->GetDisplayFont();
			notepadForm->ReplaceDisplayFont(NULL);
		}
		notepadForm->ReplaceDisplayFont(new CFont);
		notepadForm->GetDisplayFont()->CreateFontIndirect(&logFont);

		//4. SizeCalculator를 재생성한다.
		if (notepadForm->sizeCalculator != NULL)
		{
			delete notepadForm->sizeCalculator;
			notepadForm->sizeCalculator = NULL;
		}
		notepadForm->sizeCalculator = new SizeCalculator(this->parent);
		SizeCalculator* sizeCalculator = notepadForm->sizeCalculator;
		rowHeight = sizeCalculator->GetRowHeight();

		//5. 전체 줄 수를 센다.
		PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
		Long fileRowCount = pagingBuffer->CountRow(pagingBuffer->GetFileEndOffset());

		//6. 자동개행중이라면, 자동개행을 한다.
		dummyRowCount = 0;
		if (notepadForm->IsAutoWrapped())
		{
			dummyRowCount = noteWrapper.Wrap();
		}

		//7. 수직스크롤바가 있다면, max를 갱신한다.
		if (scrollController->HasVScroll())
		{
			Long max = (fileRowCount + dummyRowCount) * rowHeight;
			scrollController->ResizeVRange(max);
		}
	}
}