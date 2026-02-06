#ifndef _NOTEPADFORM_H //guard
#define _NOTEPADFORM_H
#include <afxwin.h>
#include "Subject.h"
#include "PageSetting.h"

class Glyph;
class CaretController;
class SizeCalculator;
class ScrollController;
class ClipboardController;
class PagingBuffer;
class CFindReplaceDialog;
class SearchResultController;
class HistoryBook;
class PreviewForm;
class PreviewForm;
class StatusBarController;
class MouseHandler;

class NotepadForm : public CWnd, public Subject {
public:
	NotepadForm(CWnd *parent, StatusBarController *statusBarController);
	virtual ~NotepadForm();
public:
	void Load(CString path, TCHAR*(*str), Long& count);
	void Save(CString path);
public:
	CWnd* parent;
	Glyph* note;
	CaretController* caretController;
	SizeCalculator* sizeCalculator;
	ScrollController* scrollController;
	ClipboardController* clipboardController;
	PagingBuffer* pagingBuffer;
	SearchResultController* searchResultController;
	HistoryBook* undoHistoryBook;
	HistoryBook* redoHistoryBook;
	StatusBarController* statusBarController;
	PageSetting pageSetting;
	PreviewForm* previewForm;
	MouseHandler* mouseHandler;
public:
	CFont* originalFont;
	CFont* displayFont;
	CMenu menu;
	BOOL hasFindReplaceDialog;
	BOOL isAutoWrapped;
	double magnification;
	CString path;
	CString encoding;
	BOOL isDirty;
public:
	BOOL IsCompositing() const;
	CWnd* GetParent();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnSize(UINT nType, int cx, int cy);
	void OnExitSizeMove();
	void OnPaint();
	LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	LRESULT OnImeEndComposition(WPARAM wParam, LPARAM lParam);
	void OnSetFocus(CWnd* pOldWnd);
	void OnKillFocus(CWnd* pNewWnd);
	void OnCommandRequested(UINT nID);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnEraseBkgnd(CDC *pDC);
	LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);
	LRESULT OnFindReplaceFocused(WPARAM wParam, LPARAM lParam);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	BOOL isCompositing;
	BOOL nextIsLastOnSize;
};

inline BOOL NotepadForm::IsCompositing() const {
	return this->isCompositing;
}

inline CWnd* NotepadForm::GetParent() {
	return this->parent;
}

#endif // !_NOTEPADFORM_H