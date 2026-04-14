#include <afxwin.h>
#include "ChangeInProgressCaption.h"
#include "searches/BruteForce.h"
#include "searches/CaseSensitiveComparer.h"

#pragma warning(disable:4996)

ChangeInProgressCaption::ChangeInProgressCaption(CWnd* parent) {
	this->parent = parent;
	
	CString caption;
	this->parent->GetWindowTextA(caption);

	CString inProgressCaption;
	inProgressCaption.Format("%s <贸府吝>", caption);
	this->parent->SetWindowTextA(inProgressCaption);
}

ChangeInProgressCaption::~ChangeInProgressCaption() {
	CString caption;
	this->parent->GetWindowTextA(caption);

	CaseSensitiveComparer caseSensitiveComparer;
	BruteForce bruteForce("<贸府吝>", (LPCTSTR)caption, &caseSensitiveComparer);
	
	Long(*indexes);
	Long count;
	bruteForce.DoAlgorithm(&indexes, &count);

	if (count > 0)
	{
		Long length = indexes[count - 1] - 1;
		caption = caption.Left(length);
		this->parent->SetWindowTextA(caption);
	}

	if (indexes != NULL)
	{
		delete[] indexes;
	}
}