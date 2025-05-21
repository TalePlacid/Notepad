#include "CaretController.h"
#include "Caret.h"
#include "SizeCalculator.h"
#include "Glyph.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

CaretController::CaretController(CWnd* parent) {
	this->parent = parent;
	this->caret = NULL;
}

CaretController::~CaretController() {
	if (this->caret != NULL)
	{
		delete this->caret;
	}
}

Caret* CaretController::Create() {
	Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
	Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

	Long x = 0;
	Long width = 0;
	CString character;
	Long caretWidth = 1;

	Long i = 0;
	while (i <= row->GetCurrent())
	{
		character = CString(row->GetAt(i)->MakeString().c_str());
		width = ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)character));
		x += width;
		i++;
	}

	if (width == ((NotepadForm*)(this->parent))->sizeCalculator->GetMultiByteWidth())
	{
		x -= width;
		caretWidth = width;
	}

	Long y = ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight() * (rowIndex+1);

	this->caret = new Caret(this->parent, x, y, caretWidth, ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight());

	return this->caret;
}

Caret* CaretController::Destroy() {
	if (this->caret != NULL)
	{
		delete this->caret;
		this->caret = NULL;
	}

	return this->caret;
}

void CaretController::Update(Subject *subject, string interest) {
	if (interest == "caretChanged")
	{
		if (this->caret != NULL)
		{
			delete this->caret;
			this->caret = NULL;
		}

		Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
		Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

		Long x = 0;
		SCROLLINFO scrollInfo = {};
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_POS;
		if (GetScrollInfo(this->parent->GetSafeHwnd(), SB_HORZ, &scrollInfo))
		{
			x -= scrollInfo.nPos;
		}

		Long caretWidth = 1;
		Long width = 0;
		CString character;
		Long i = 0;
		while (i <= row->GetCurrent())
		{
			character = CString(row->GetAt(i)->MakeString().c_str());
			width = ((NotepadForm*)(this->parent))->sizeCalculator->GetCharacterWidth(const_cast<char*>((LPCTSTR)character));
			x += width;
			i++;
		}

		if (width == ((NotepadForm*)(this->parent))->sizeCalculator->GetMultiByteWidth())
		{
			x -= width;
			caretWidth = width;
		}

		Long y = ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight() * rowIndex;

		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_POS;
		if (GetScrollInfo(this->parent->GetSafeHwnd(), SB_VERT, &scrollInfo))
		{
			y -= scrollInfo.nPos;
		}

		this->caret = new Caret(this->parent, x, y, caretWidth, ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight());
	}
}