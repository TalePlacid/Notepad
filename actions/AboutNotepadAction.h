#ifndef _ABOUTNOTEPADACTION_H //guard
#define _ABOUTNOTEPADACTION_H
#include "Action.h"

class CWnd;

class AboutNotepadAction : public Action {
public:
	AboutNotepadAction(CWnd* parent);
	virtual ~AboutNotepadAction();

	virtual void Perform();
	virtual bool ShouldKeepSelection();
};

inline bool AboutNotepadAction::ShouldKeepSelection() {
	return true;
}

#endif // !_ABOUTNOTEPADACTION_H