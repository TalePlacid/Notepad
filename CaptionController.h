#ifndef _CAPTIONCONTROLLER_H //guard
#define _CAPTIONCONTROLLER_H
#include "Observer.h"

class CWnd;

class CaptionController : public Observer {
public:
	CaptionController(CWnd *parent);
	virtual ~CaptionController();

	void UpdateCaption();

	virtual void Update(Subject* subject, string interest);
private:
	CWnd* parent;
	CString caption;
};

#endif // !_CAPTIONCONTROLLER_H
