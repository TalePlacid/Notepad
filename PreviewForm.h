#ifndef _PREVIEWFORM_H //guard
#define _PREVIEWFORM_H
#include <afxwin.h>

typedef signed long int Long;

class PreviewLayout;
class PreviewScaler;
class PreviewPaginator;

class PreviewForm : public CFrameWnd {
public:
	PreviewForm(CWnd *parent);
	virtual ~PreviewForm();
protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	CWnd* parent;
};

#endif // !_PREVIEWFORM_H
