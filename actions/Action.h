#ifndef _ACTION_H //guard
#define _ACTION_H

class CWnd;

class Action {
public:
	Action(CWnd* parent);
	virtual ~Action() = 0;

	virtual void Perform() = 0;
	virtual bool NeedScrollBarUpdate();
	virtual bool NeedUpdateLatestPoint();
	virtual bool NeedsNoteTruncation();
	virtual bool ShouldKeepSelection();
protected:
	CWnd* parent;
};

inline bool Action::NeedScrollBarUpdate() {
	return true;
}

inline bool Action::NeedUpdateLatestPoint() {
	return false;
}

inline bool Action::NeedsNoteTruncation() {
	return false;
}

inline bool Action::ShouldKeepSelection() {
	return false;
}

#endif // !_ACTION_H

