#include "Caret.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

Caret::Caret(CWnd* parent, Long x, Long y, Long width, Long height) {
	this->parent = parent;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	::CreateCaret(this->parent->GetSafeHwnd(), NULL, this->width, this->height);
	::SetCaretPos(this->x, this->y);
	::ShowCaret(this->parent->GetSafeHwnd());
}

Caret::~Caret() {
	::DestroyCaret();
}
