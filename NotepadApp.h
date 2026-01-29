#ifndef _NOTEPADAPP_H //guard
#define _NOTEPADAPP_H
#include <afxwin.h>

class NotepadApp : public CWinApp {
public:
	NotepadApp();
	~NotepadApp();

	virtual BOOL InitInstance();
};

#endif // !_NOTEPADAPP_H
