#ifndef _PAGESETTINGFORM_H //guard
#define _PAGESETTINGFORM_H
#include <afxwin.h>
#include "../resource.h"

typedef signed long int Long;

class PageSettingLayout;

class PageSettingForm : public CDialog {
public:
	enum {IDD = IDD_DIALOG_PAGESETTING};
public:
	PageSettingLayout* pageSettingLayout;
public:
	PageSettingForm(CWnd* parent = NULL);
	~PageSettingForm();

	virtual BOOL OnInitDialog();
protected:
	void OnOptionChanged();
	void OnPaint();
	virtual void OnOK();
	virtual void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CWnd* parent;
	BOOL isInitialized;
};


#endif // !_PAGESETTINGFORM_H
          