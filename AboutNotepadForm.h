#ifndef _ABOUTNOTEPADFORM_H //guard
#define _ABOUTNOTEPADFORM_H
#include <afxwin.h>
#include "resource.h"

typedef signed long int Long;

class AboutNotepadForm : public CDialog {
public:
	enum { IDD = IDD_DIALOG_ABOUT_NOTEPAD };

	AboutNotepadForm(CWnd* parent = NULL);
	virtual ~AboutNotepadForm();

	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	CWnd* parent;
};

#endif // !_ABOUTNOTEPADFORM_H
