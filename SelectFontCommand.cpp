#include <afxdlgs.h>
#include "SelectFontCommand.h"
#include "Font.h"
#include "NotepadForm.h"
#include "SizeCalculator.h"
#include "ScrollBarAnalyzer.h"
#include "ScrollController.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

SelectFontCommand::SelectFontCommand(CWnd* parent)
	:Command(parent) {
	this->oldFont = NULL;
}

SelectFontCommand::~SelectFontCommand() {
	if (this->oldFont != NULL)
	{
		delete this->oldFont;
	}
}

SelectFontCommand::SelectFontCommand(const SelectFontCommand& source)
	:Command(source) {
	this->oldFont = new Font(*source.oldFont);
}

SelectFontCommand& SelectFontCommand::operator=(const SelectFontCommand& source) {
	Command::operator=(source);
	if (this->oldFont != NULL)
	{
		delete this->oldFont;
		this->oldFont = NULL;
	}

	this->oldFont = new Font(*source.oldFont);

	return *this;
}

void SelectFontCommand::Execute() {
	CFontDialog cFontDialog;
	INT_PTR result = cFontDialog.DoModal();
	if (result == IDOK)
	{
		LOGFONT logFont;
		cFontDialog.GetCurrentFont(&logFont);
		CFont* cFont = new CFont;
		cFont->CreateFontIndirectA(&logFont);

		if (((NotepadForm*)(this->parent))->font != NULL)
		{
			this->oldFont = new Font(*((NotepadForm*)(this->parent))->font);
			delete ((NotepadForm*)(this->parent))->font;
			((NotepadForm*)(this->parent))->font = NULL;
		}

		((NotepadForm*)(this->parent))->font = new Font(cFontDialog.GetFaceName(), cFontDialog.GetSize(), cFontDialog.GetStyleName(), cFont);

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

void SelectFontCommand::Unexecute() {
	if (((NotepadForm*)(this->parent))->font != NULL)
	{
		delete ((NotepadForm*)(this->parent))->font;
		((NotepadForm*)(this->parent))->font = NULL;
	}
	((NotepadForm*)(this->parent))->font = new Font(*this->oldFont);

	if (((NotepadForm*)(this->parent))->sizeCalculator != NULL)
	{
		delete ((NotepadForm*)(this->parent))->sizeCalculator;
		((NotepadForm*)(this->parent))->sizeCalculator = NULL;
	}

	((NotepadForm*)(this->parent))->sizeCalculator = new SizeCalculator(this->parent);
}