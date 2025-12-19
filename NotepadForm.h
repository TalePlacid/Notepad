#ifndef _NOTEPADFORM_H //guard
#define _NOTEPADFORM_H
#include <afxwin.h>
#include "Subject.h"

class Glyph;
class CaretController;
class Font;
class SizeCalculator;
class ScrollController;
class ClipboardController;
class PagingBuffer;
class CFindReplaceDialog;
class SearchResultController;
class HistoryBook;
class PreviewForm;

class NotepadForm : public CFrameWnd, public Subject {
public:
	NotepadForm();
	~NotepadForm();
public:
	CString Load(CString path);
	void Save(CString path);
public:
	Glyph* note;
	CaretController* caretController;
	SizeCalculator* sizeCalculator;
	Font* font;
	CMenu menu;
	ScrollController* scrollController;
	ClipboardController* clipboardController;
	PagingBuffer* pagingBuffer;
	SearchResultController* searchResultController;
	HistoryBook* undoHistoryBook;
	HistoryBook* redoHistoryBook;
	BOOL hasFindReplaceDialog;
	BOOL isAutoWrapped;
public:
	BOOL IsCompositing() const;
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

#endif // !_NOTEPADFORM_H