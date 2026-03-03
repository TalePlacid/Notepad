#ifndef _EDITOR_H //guard
#define _EDITOR_H
#include <afxwin.h>
#include "FindReplaceOption.h"

typedef signed long int Long;

class Editor {
public:
	Editor(CWnd* parent);
	~Editor();

	void InsertTextAt(Long offset, Long columnIndex, CString text, BOOL isSelected = FALSE);
	void EraseRange(Long frontOffset, Long rearOffset, Long& columnIndex);
	void Replace(Long offset, CString sourceText, CString replacingText, 
		BOOL isSelected, Long& columnIndex);
	void MoveUp();
	void MoveDown();
	void DragUp(CPoint point);
	void DragDown(CPoint point);
	void DragLeft(CPoint point);
	void DragRight(CPoint point);

	Long Find(FindReplaceOption findReplaceOption);
	bool FindPrevious();
	bool FindNext();

	bool GetSelectedRange(Long& frontOffset, Long& rearOffset);
private:
	CWnd* parent;
};

#endif // !_EDITOR_H

