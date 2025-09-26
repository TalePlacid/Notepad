#ifndef _PREVIEWFORM_H //guard
#define _PREVIEWFORM_H
#include <afxwin.h>
#include "resource.h"

#define INCH_TO_MM 25.4

class PreviewForm : public CDialog {
public:
	enum {IDD=IDD_DIALOG_PREVIEW};
public:
	PreviewForm(CWnd* parent = NULL);
	~PreviewForm();

	virtual BOOL OnInitDialog();
protected:
	void OnOptionChanged();
	void OnPaint();
	virtual void OnClose();
	DECLARE_MESSAGE_MAP()
};



#endif // !_PREVIEWFORM_H
