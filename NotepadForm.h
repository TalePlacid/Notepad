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

class NotepadForm : public CWnd, public Subject {
public:
	NotepadForm(CWnd *parent, StatusBarController *statusBarController);
	virtual ~NotepadForm();
public:
	CString Load(CString path);
	void Save(CString path);
public:
	CWnd* parent;
	Glyph* note;
	CaretController* caretController;
	SizeCalculator* sizeCalculator;
	CFont* originalFont;
	CFont* displayFont;
	CMenu menu;
	ScrollController* scrollController;
	ClipboardController* clipboardController;
	PagingBuffer* pagingBuffer;
	SearchResultController* searchResultController;
	HistoryBook* undoHistoryBook;
	HistoryBook* redoHistoryBook;
	StatusBarController* statusBarController;
	PageSetting pageSetting;
	PreviewForm* previewForm;
	BOOL hasFindReplaceDialog;
	BOOL isAutoWrapped;
	double magnification;
public:
	BOOL IsCompositing() const;
	CWnd* GetParent();
	CString GetPath() const;
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
	void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	BOOL isCompositing;
	CString path;
	BOOL nextIsLastOnSize;
};

inline BOOL NotepadForm::IsCompositing() const {
	return this->isCompositing;
}

inline CString NotepadForm::GetPath() const {
	return const_cast<CString&>(this->path);
}

inline CWnd* NotepadForm::GetParent() {
	return this->parent;
}

#endif // !_NOTEPADFORM_H