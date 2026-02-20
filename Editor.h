#ifndef _EDITOR_H //guard
#define _EDITOR_H

typedef signed long int Long;

class CWnd;

class Editor {
public:
	Editor(CWnd* parent);
	~Editor();

	void InsertTextAt(Long offset, Long columnIndex, CString text, bool isSelected = false);
	void EraseRange(Long frontOffset, Long rearOffset, Long& columnIndex, CString& contents);

	bool GetSelectedRange(Long& frontOffset, Long& rearOffset);
private:
	CWnd* parent;
};

#endif // !_EDITOR_H
