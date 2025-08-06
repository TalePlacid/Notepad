#ifndef _MARKINGHELPER_H //guard
#define _MARKINGHELPER_H

class CWnd;

class MarkingHelper {
public:
	MarkingHelper(CWnd* parent);
	~MarkingHelper();

	void Mark();
	void Unmark();

	bool IsUnmarked();
	bool HasReturnedToSelectionBegin();
private:
	CWnd* parent;
};

#endif // !_MARKINGHELPER_H
