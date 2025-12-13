#ifndef _ACTION_H //guard
#define _ACTION_H

class CWnd;

class Action {
public:
	Action(CWnd* parent);
	virtual ~Action() = 0;

	virtual void Perform() = 0;
	virtual bool NeedScrollBarUpdate();
protected:
	CWnd* parent;
};

inline bool Action::NeedScrollBarUpdate() {
	return true;
}

#endif // !_ACTION_H
