#ifndef _KEYACTION_H //guard
#define _KEYACTION_H
#include "Action.h"

class CWnd;

class KeyAction : public Action {
public:
	KeyAction(CWnd* parent);
	virtual ~KeyAction() = 0;

	virtual bool ShouldKeepSelection() const;
};

inline bool KeyAction::ShouldKeepSelection() const {
	return false;
}

#endif // !_KEYDOWNACTION_H
