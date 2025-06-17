#ifndef _NOTEPADFORM_H //guard
#define _NOTEPADFORM_H
#include <afxwin.h>
#include "Subject.h"

class Glyph;
class CaretController;
class Font;
class SizeCalculator;
class ScrollBarController;
class ClipboardController;
class PagingBuffer;

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
	ScrollBarController* scrollBarController;
	ClipboardController* clipboardController;
	PagingBuffer* pagingBuffer;
public:
	BOOL IsCompositing() const;
	CString GetPath() const;
protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnSize(UINT nType, int cx, int cy);
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
	void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	BOOL isCompositing;
	CString path;
};

inline BOOL NotepadForm::IsCompositing() const {
	return this->isCompositing;
}

inline CString NotepadForm::GetPath() const {
	return const_cast<CString&>(this->path);
}

#endif // !_NOTEPADFORM_H