#ifndef _GOTOFORM_H //guard
#define _GOTOFORM_H
#include <afxwin.h>
#include "resource.h"

typedef signed long int Long;

class GoToForm : public CDialog {
public:
	enum { IDD = IDD_DIALOG_GOTO };

	GoToForm(CWnd* parent = NULL);
	virtual ~GoToForm();

	Long GetLineNumber() const;
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CWnd* parent;
	Long lineNumber;
};

inline Long GoToForm::GetLineNumber() const {
	return this->lineNumber;
}

#endif // !_GOTOFORM_H
