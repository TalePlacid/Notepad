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
	BOOL IsCompositing() const;
	CWnd* GetParent();
	CFont* GetOriginalFont() const;
	void ReplaceOriginalFont(CFont* font);
	CFont* GetDisplayFont() const;
	void ReplaceDisplayFont(CFont* font);
	BOOL IsAutoWrapped() const;
	void EnableAutoWrap();
	void DisableAutoWrap();
	void ToggleAutoWrap();
	double GetMagnification() const;
	void ChangeMagnification(double magnification);
	CString GetSourcePath() const;
	void AssignSourcePath(CString sourcePath);
	Encoding GetEncoding() const;
	void ApplyEncoding(Encoding encoding);
	BOOL IsDirty() const;
	void MarkDirty();
	void MarkClean();
	PageSetting GetPageSetting() const;
	void ApplyPageSetting(const PageSetting& pageSetting);
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
	CFont* originalFont;
	CFont* displayFont;
	BOOL isAutoWrapped;
	double magnification;
	CString sourcePath;
	Encoding encoding;
	BOOL isDirty;
	BOOL isCompositing;
	BOOL nextIsLastOnSize;
};

inline BOOL NotepadForm::IsCompositing() const {
	return this->isCompositing;
}

inline CWnd* NotepadForm::GetParent() {
	return this->parent;
}

inline CFont* NotepadForm::GetOriginalFont() const {
	return this->originalFont;
}

inline void NotepadForm::ReplaceOriginalFont(CFont* font) {
	this->originalFont = font;
}

inline CFont* NotepadForm::GetDisplayFont() const {
	return this->displayFont;
}

inline void NotepadForm::ReplaceDisplayFont(CFont* font) {
	this->displayFont = font;
}

inline BOOL NotepadForm::IsAutoWrapped() const {
	return this->isAutoWrapped;
}

inline void NotepadForm::EnableAutoWrap() {
	this->isAutoWrapped = TRUE;
}

inline void NotepadForm::DisableAutoWrap() {
	this->isAutoWrapped = FALSE;
}

inline void NotepadForm::ToggleAutoWrap() {
	this->isAutoWrapped = !this->isAutoWrapped;
}

inline double NotepadForm::GetMagnification() const {
	return this->magnification;
}

inline void NotepadForm::ChangeMagnification(double magnification) {
	this->magnification = magnification;
}

inline CString NotepadForm::GetSourcePath() const {
	return this->sourcePath;
}

inline void NotepadForm::AssignSourcePath(CString sourcePath) {
	this->sourcePath = sourcePath;
}

inline Encoding NotepadForm::GetEncoding() const {
	return this->encoding;
}

inline void NotepadForm::ApplyEncoding(Encoding encoding) {
	this->encoding = encoding;
}

inline BOOL NotepadForm::IsDirty() const {
	return this->isDirty;
}

inline void NotepadForm::MarkDirty() {
	this->isDirty = TRUE;
}

inline void NotepadForm::MarkClean() {
	this->isDirty = FALSE;
}

inline PageSetting NotepadForm::GetPageSetting() const {
	return this->pageSetting;
}

inline void NotepadForm::ApplyPageSetting(const PageSetting& pageSetting) {
	this->pageSetting = pageSetting;
}

#endif // !_NOTEPADFORM_H

