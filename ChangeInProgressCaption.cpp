#include <afxwin.h>
#include "ChangeInProgressCaption.h"
#include "searches/BruteForce.h"
#include "searches/CaseSensitiveComparer.h"

#pragma warning(disable:4996)

ChangeInProgressCaption::ChangeInProgressCaption() {

}

ChangeInProgressCaption::~ChangeInProgressCaption() {
	
}

void ChangeInProgressCaption::AddInProgressCaption(CWnd* parent) {
	CString caption;
	parent->GetWindowTextA(caption);

	CString inProgressCaption;
	inProgressCaption.Format("%s <贸府吝>", caption);
	parent->SetWindowTextA(inProgressCaption);
}

void ChangeInProgressCaption::RemoveInProgressCaption(CWnd* parent) {
	CString caption;
	parent->GetWindowTextA(caption);

	CaseSensitiveComparer caseSensitiveComparer;
	BruteForce bruteForce("<贸府吝>", (LPCTSTR)caption, &caseSensitiveComparer);

	Long(*indexes);
	Long count;
	bruteForce.DoAlgorithm(&indexes, &count);

	if (count > 0)
	{
		Long length = indexes[count - 1] - 1;
		caption = caption.Left(length);
		parent->SetWindowTextA(caption);
	}

	if (indexes != NULL)
	{
		delete[] indexes;
	}
}