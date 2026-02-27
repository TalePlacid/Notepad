#ifndef _NOTEPADFORM_H //guard
#define _NOTEPADFORM_H
#include <afxwin.h>
#include "AppID.h"
#include "Subject.h"
#include "prints/PageSetting.h"
#include "Encoding.h"

typedef unsigned long int ULong;

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
class FindReplaceOption;
class CFindReplaceDialog;

class NotepadForm : public CWnd, public Subject {
public:
	NotepadForm(CWnd *parent, CString sourcePath, StatusBarController *statusBarController);
	virtual ~NotepadForm();
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
	CFindReplaceDialog* findReplaceDialog;
public:
	CFont* originalFont;
	CFont* displayFont;
	BOOL isAutoWrapped;
	double magnification;
	CString sourcePath;
	Encoding encoding;
	BOOL isDirty;
public:
	BOOL IsCompositing() const;
	CWnd* GetParent();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	LRESULT OnImeEndComposition(WPARAM wParam, LPARAM lParam);
	void OnSize(UINT nType, int cx, int cy);
	void OnExitSizeMove();
	void OnPaint();
	void OnSetFocus(CWnd* pOldWnd);
	void OnKillFocus(CWnd* pNewWnd);
	void OnMenuClicked(UINT nID);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnEraseBkgnd(CDC *pDC);
	LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);
	LRESULT OnFindReplaceFocused(WPARAM wParam, LPARAM lParam);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
private:
	void ResolveFindReplaceRequest(AppID appID, FindReplaceOption& findReplaceOption);
	void HandleCommand(AppID nID, const TCHAR(*character) = 0, BOOL onChar = TRUE,
		FindReplaceOption* findReplaceOption = NULL);
	void HandleAction(AppID nID, FindReplaceOption* findReplaceOption = NULL);
#if 0
	void HandleMouseEvent(AppID nID, UINT nFlags, CPoint point, short zDelta = 0);
#endif
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