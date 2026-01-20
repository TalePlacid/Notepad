#ifndef _PREVIEWFORM_H //guard
#define _PREVIEWFORM_H
#include <afxwin.h>

typedef signed long int Long;

class PreviewLayout;
class PreviewScaler;
class PreviewPaginator;

class PreviewForm : public CFrameWnd {
public:
	CWnd* parent;
	PreviewLayout* previewLayout;
	PreviewScaler* previewScaler;
	PreviewPaginator* previewPaginator;
public:
	PreviewForm(CWnd *parent);
	virtual ~PreviewForm();
protected:
	CButton firstButton;
	CButton previousButton;
	CButton nextButton;
	CButton lastButton;
	CStatic pageNumber;
	CFont* pageNumberFont;
protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSize(UINT nType, int cx, int cy);
	void OnPaint();
	void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif // !_PREVIEWFORM_H
