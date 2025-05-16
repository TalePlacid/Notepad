#ifndef _KEYACTION_H //guard
#define _KEYACTION_H

class CWnd;

class KeyAction {
public:
	KeyAction(CWnd* parent);
	virtual ~KeyAction() = 0;

	virtual void Perform() = 0;
protected:
	CWnd* parent;
};

#endif // !_KEYDOWNACTION_H
