#include <afxwin.h>
#include "ChangeInProgressCaption.h"

#pragma warning(disable:4996)

ChangeInProgressCaption::ChangeInProgressCaption(CWnd* parent) {
	this->parent = parent;
	
	this->parent->GetWindowTextA(this->caption);

	CString inProgressCaption;
	inProgressCaption.Format("%s *Ã³¸®Áß*", this->caption);
	this->parent->SetWindowTextA(inProgressCaption);
}

ChangeInProgressCaption::~ChangeInProgressCaption() {
	this->parent->SetWindowTextA(this->caption);
}