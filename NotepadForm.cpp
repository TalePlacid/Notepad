#include <afxdlgs.h>
#include <imm.h>

#include "NotepadForm.h"
#include "resource.h"
#include "message.h"
#include "Observer.h"

#include "glyphs/Note.h"
#include "PagingBuffer.h"
#include "SizeCalculator.h"
#include "CaretController.h"
#include "ScrollController.h"
#include "HistoryBook.h"
#include "SearchResultController.h"
#include "StatusBarController.h"
#include "MouseHandler.h"
#include "ClipboardController.h"
#include "TextFileIO.h"

#include "NoteConverter.h"
#include "NoteWrapper.h"
#include "TextOutVisitor.h"
#include "EncodingDetector.h"
#include "TextEncoder.h"
#include "CoordinateConverter.h"
#include "AutoScroller.h"
#include "WritingModeSelector.h"
#include "KeyDownInterpreter.h"
#include "MenuInterpreter.h"
#include "FindReplaceInterpreter.h"
#include "FindReplaceOptionMaker.h"
#include "FindReplaceRequestResolver.h"

#include "glyphs/GlyphFactory.h"
#include "commands/CommandFactory.h"
#include "actions/ActionFactory.h"
#include "scrolls/ScrollBarActionFactory.h"
#include "mouses/MouseActionFactory.h"

#include "prints/PreviewForm.h"
#include "Caret.h"
#include "glyphs/Glyph.h"
#include "commands/Command.h"
#include "actions/Action.h"
#include "scrolls/ScrollBarAction.h"
#include "mouses/MouseAction.h"

#pragma warning(disable:4996)
#pragma comment(lib, "imm32.lib")

static const UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

BEGIN_MESSAGE_MAP(NotepadForm, CWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_SIZE()
	ON_WM_EXITSIZEMOVE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_ENDCOMPOSITION, OnImeEndComposition)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND_RANGE(ID_MENU_UNDO, ID_MENU_SETPAGE, OnMenuClicked)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
	ON_MESSAGE(WM_FINDREPLACE_FOCUS, OnFindReplaceFocused)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	END_MESSAGE_MAP()


NotepadForm::NotepadForm(CWnd *parent, CString sourcePath, StatusBarController* statusBarController) {
	this->parent = parent;
	this->note = NULL;
	this->isCompositing = FALSE;
	this->ReplaceOriginalFont(NULL);
	this->ReplaceDisplayFont(NULL);
	this->sizeCalculator = NULL;
	this->caretController = NULL;
	this->scrollController = NULL;
	this->clipboardController = NULL;
	this->pagingBuffer = NULL;
	this->searchResultController = NULL;
	this->undoHistoryBook = NULL;
	this->redoHistoryBook = NULL;
	this->findReplaceDialog = NULL;
	this->previewForm = NULL;
	this->statusBarController = statusBarController;
	this->mouseHandler = NULL;
	this->isCompositing = FALSE;
	this->DisableAutoWrap();
	this->ChangeMagnification(1.0);
	this->MarkClean();

	this->sourcePath = sourcePath;
}

NotepadForm::~NotepadForm() {
	if (this->note != NULL)
	{
		delete this->note;
		this->note = NULL;
	}

	if (this->caretController != NULL)
	{
		delete this->caretController;
		this->caretController = NULL;
	}

	if (this->originalFont != NULL)
	{
		delete this->GetOriginalFont();
	}

	if (this->displayFont != NULL)
	{
		delete this->GetDisplayFont();
	}

	if (this->sizeCalculator != NULL)
	{
		delete this->sizeCalculator;
	}

	if (this->scrollController != NULL)
	{
		delete this->scrollController;
	}

	if (this->clipboardController != NULL)
	{
		delete this->clipboardController;
	}

	if (this->pagingBuffer != NULL)
	{
		delete this->pagingBuffer;
	}

	if (this->searchResultController != NULL)
	{
		delete this->searchResultController;
	}

	if (this->undoHistoryBook != NULL)
	{
		delete this->undoHistoryBook;
	}

	if (this->redoHistoryBook != NULL)
	{
		delete this->redoHistoryBook;
	}

	if (this->previewForm != NULL)
	{
		delete this->previewForm;
	}

	if (this->mouseHandler != NULL)
	{
		delete this->mouseHandler;
	}
}

BOOL NotepadForm::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style &= ~WS_HSCROLL;
	cs.style &= ~WS_VSCROLL;

	return CWnd::PreCreateWindow(cs);
}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);
	
	//기본 폰트(맑은 고딕, 10pt, 보통 스타일)
	LOGFONT logFont = { 0, };
	CClientDC dc(this);
	logFont.lfHeight = -MulDiv(10, dc.GetDeviceCaps(LOGPIXELSY), 72);
	logFont.lfWeight = FW_NORMAL;
	logFont.lfCharSet = HANGUL_CHARSET;
	_tcscpy_s(logFont.lfFaceName, "맑은 고딕");

	this->ReplaceOriginalFont(new CFont);
	this->GetOriginalFont()->CreateFontIndirectA(&logFont);

	double round = 0.5;
	if (logFont.lfHeight < 0)
	{
		round = -0.5;
	}
	logFont.lfHeight = logFont.lfHeight * this->GetMagnification() + round;
	this->ReplaceDisplayFont(new CFont);
	this->GetDisplayFont()->CreateFontIndirectA(&logFont);

	this->sizeCalculator = new SizeCalculator(this);

	TextFileIO textFileIO;
	TCHAR(*sourceContents) = 0;
	Long sourceCount;
	textFileIO.Load((LPCTSTR)(this->sourcePath), &sourceContents, sourceCount);

	this->pagingBuffer = new PagingBuffer(this, sourceContents, sourceCount);
	if (sourceContents != NULL)
	{
		delete[] sourceContents;
	}

	this->note = this->pagingBuffer->LoadNext();

	this->caretController = new CaretController(this);
	this->Register(this->caretController);

	this->scrollController = new ScrollController(this);
	this->Register(this->scrollController);

	this->clipboardController = new ClipboardController(this);

	this->searchResultController = new SearchResultController(this);
	this->undoHistoryBook = new HistoryBook;
	this->redoHistoryBook = new HistoryBook;

	this->statusBarController->Create();
	this->Register(this->statusBarController);

	this->mouseHandler = new MouseHandler(this);

	Margin margin;
	margin.left = 20;
	margin.right = 20;
	margin.up = 25;
	margin.down = 25;
	this->pageSetting = PageSetting(CString("A4"), TRUE, margin, CString(""), CString(""));

	this->nextIsLastOnSize = FALSE;

	if (this->sourcePath != "")
	{
		Long index = this->sourcePath.ReverseFind('\\');
		CString fileName = this->sourcePath.Right(this->sourcePath.GetLength() - (index + 1));
		Long extensionIndex = fileName.ReverseFind('.');
		if (extensionIndex > 0)
		{
			fileName = fileName.Left(extensionIndex);
		}
		this->parent->SetWindowTextA((LPCTSTR)fileName);
	}

	this->Notify("UpdateScrollBars");

	return 0;
}

void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if ((nChar > 31 && nChar < 127) || nChar == 9 || nChar == 13)
	{
		AppID nID = WritingModeSelector::DetermineWritingMode(this->pagingBuffer);
		TCHAR character[2];
		character[0] = nChar;
		character[1] = '\0';
		if (character[0] == '\r')
		{
			character[1] = '\n';
		}
		this->HandleCommand(nID, character, TRUE);
	}
}

LRESULT NotepadForm::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {
	return TRUE;
}

LRESULT NotepadForm::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	if (lParam & GCS_COMPSTR)
	{
		HIMC himc = ImmGetContext(this->GetSafeHwnd());
		TCHAR character[256];
		Command* command = NULL;
		Long length = ImmGetCompositionString(himc, GCS_COMPSTR, character, 256);
		character[length] = '\0';

		if (length > 0)
		{
			AppID nID = WritingModeSelector::DetermineWritingMode(this->pagingBuffer);
			this->HandleCommand(nID, character, FALSE);
			this->isCompositing = TRUE;
		}
		else if (length == 0)
		{
			this->HandleCommand(AppID::ID_COMMAND_ERASE, NULL, FALSE);
			this->isCompositing = FALSE;
		}

		ImmReleaseContext(this->GetSafeHwnd(), himc);
	}

	return DefWindowProc(WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam) {
	char character[2];
	character[0] = (BYTE)(wParam >> 8);
	character[1] = (BYTE)wParam;

	AppID nID = WritingModeSelector::DetermineWritingMode(this->pagingBuffer);
	this->HandleCommand(nID, character, TRUE);

	return 0;
}

LRESULT NotepadForm::OnImeEndComposition(WPARAM wParam, LPARAM lParam) {
	this->isCompositing = FALSE;

	return TRUE;
}

void NotepadForm::OnSize(UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);

	if (this->IsAutoWrapped())
	{
		NoteWrapper noteWrapper(this);
		noteWrapper.Unwrap();
		noteWrapper.Wrap();
	}

	if (this->nextIsLastOnSize)
	{
		this->Notify("UpdateScrollBars");
	}
}

void NotepadForm::OnExitSizeMove() {
	this->nextIsLastOnSize = TRUE;
}

void NotepadForm::OnPaint() {
	CPaintDC dc(this);

	RECT rect;
	this->GetClientRect(&rect);
	Long width = rect.right - rect.left;
	Long height = rect.bottom - rect.top;

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(&dc, width, height);

	CBitmap* oldBitMap = memDC.SelectObject(&bitMap);

	memDC.FillSolidRect(&rect, RGB(255, 255, 255));
	TextOutVisitor textOutVisitor(this, &memDC);
	this->note->Accept(textOutVisitor);

	dc.BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(oldBitMap);

	if (CWnd::GetFocus() == this)
	{
		this->Notify("ChangeCaret");
	}
}

void NotepadForm::OnSetFocus(CWnd* pOldWnd) {
	if (this->caretController != NULL)
	{
		this->Notify("ChangeCaret");
	}
}

void NotepadForm::OnKillFocus(CWnd* pNewWnd) {
	if (this->caretController != NULL)
	{
		this->caretController->Destroy();
	}

	this->Invalidate();
}

void NotepadForm::OnMenuClicked(UINT nID) {
	AppID appID = MenuInterpreter::DetermineID(nID);
	if (appID != AppID::NONE)
	{
		if (MenuInterpreter::IsFindReplace(nID))
		{
			FindReplaceOption findReplaceOption;
			this->ResolveFindReplaceRequest(appID, findReplaceOption);
		}
		else if (MenuInterpreter::IsCommand(nID))
		{
			this->HandleCommand(appID);
		}
		else
		{
			this->HandleAction(appID);
		}
	}
}

void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	AppID nID = KeyDownInterpreter::DetermineID(nChar);
	if (nID != AppID::NONE)
	{
		if (KeyDownInterpreter::IsFindReplace(nID))
		{
			FindReplaceOption findReplaceOption;
			this->ResolveFindReplaceRequest(nID, findReplaceOption);
		}
		else if (KeyDownInterpreter::IsCommand(nID))
		{
			this->HandleCommand(nID);
		}
		else
		{
			this->HandleAction(nID);
		}
	}
}

void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollBarActionFactory scrollBarActionFactory;
	ScrollBarAction* scrollBarAction = scrollBarActionFactory.Create(this, SB_VERT, nSBCode, nPos);
	if (scrollBarAction != NULL)
	{
		scrollBarAction->Perform();
		if (!scrollBarAction->ShouldKeepSelection())
		{
			this->note->Select(false);
			this->pagingBuffer->UnmarkSelectionBegin();
		}
		delete scrollBarAction;
	}

	this->Invalidate();
}

void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollBarActionFactory scrollBarActionFactory;
	ScrollBarAction* scrollBarAction = scrollBarActionFactory.Create(this, SB_HORZ, nSBCode, nPos);
	if (scrollBarAction != NULL)
	{
		scrollBarAction->Perform();
		delete scrollBarAction;
	}

	this->Invalidate();
}

BOOL NotepadForm::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}
LRESULT NotepadForm::OnFindReplace(WPARAM wParam, LPARAM lParam) {
	CFindReplaceDialog* findReplaceDialog = CFindReplaceDialog::GetNotifier(lParam);
	AppID rawID = FindReplaceInterpreter::DetermineID(findReplaceDialog);
	FindReplaceOption findReplaceOption = FindReplaceOptionMaker::Make(findReplaceDialog);

	this->ResolveFindReplaceRequest(rawID, findReplaceOption);

	return 0;
}

LRESULT NotepadForm::OnFindReplaceFocused(WPARAM wParam, LPARAM lParam) {
	CFindReplaceDialog* findReplaceForm = (CFindReplaceDialog*)wParam;
	findReplaceForm->SetFocus();

	return 0;
}

void NotepadForm::ResolveFindReplaceRequest(AppID appID, FindReplaceOption& findReplaceOption) {
	//1. 옵션이 없으면, 보충한다.
	FindReplaceRequestResolver resolver(this);
	resolver.SupplementOption(findReplaceOption);

	//1. 문맥에 맞는 앱 아이디를 판별한다.
	AppID nID = resolver.ResolveAppID(appID, findReplaceOption);

	//3. 앱 아이디에 따라 핸들러로 이관한다.
	if (resolver.IsCommand(nID))
	{
		this->HandleCommand(nID, 0, 0, &findReplaceOption);
	}
	else
	{
		this->HandleAction(nID, &findReplaceOption);
	}
}

void NotepadForm::OnLButtonDown(UINT nFlags, CPoint point) {
	//this->HandleMouseEvent(WM_LBUTTONDOWN, nFlags, point);
}

void NotepadForm::OnMouseMove(UINT nFlags, CPoint point) {
	if (nFlags == MK_LBUTTON && this->mouseHandler->OnDrag())
	{
		AutoScroller autoScroller(this);
		BOOL isAutoScrolled = autoScroller.ScrollIfNeeded(point);
		if (isAutoScrolled)
		{
			SetTimer(0, 30, NULL);
		}

		//this->HandleMouseEvent(WM_MOUSEMOVE, nFlags, point);
	}
}

void NotepadForm::OnLButtonUp(UINT nFlags, CPoint point) {
	this->mouseHandler->EndDrag();
	KillTimer(0);
}

BOOL NotepadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	//this->HandleMouseEvent(WM_MOUSEWHEEL, nFlags, pt, zDelta);

	return 0;
}

void NotepadForm::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent)
	{
	case 0:
	{
		CPoint cursorPos;
		GetCursorPos(&cursorPos);
		AutoScroller autoScroller(this);
		autoScroller.ScrollIfNeeded(cursorPos);

		//this->HandleMouseEvent(WM_MOUSEMOVE, MK_LBUTTON, cursorPos);
	}
		break;
	default:
		break;
	}
}

void NotepadForm::OnRButtonDown(UINT nFlags, CPoint point) {
	ClientToScreen(&point);
	this->mouseHandler->PopUpContextMenu(point);
}

void NotepadForm::HandleCommand(AppID nID, const TCHAR(*character), BOOL onChar, 
	FindReplaceOption* findReplaceOption) {
	BOOL isSelected = this->pagingBuffer->GetSelectionBeginOffset() >= 0;

	Command* command = CommandFactory::Create(this, nID, character, onChar,
		isSelected, findReplaceOption);
	if (command != NULL)
	{
		command->Execute();

		if (command->NeedScrollBarUpdate())
		{
			this->Notify("UpdateScrollBars");
		}

		if (command->IsUndoable())
		{
			command = this->undoHistoryBook->Bind(command);
			this->undoHistoryBook->Push(command);
			this->redoHistoryBook->Clear();
			this->MarkDirty();
		}
		else
		{
			delete command;
		}
	}

	this->MarkDirty();
	this->Notify("UpdateStatusBar");
	this->Invalidate();
}

void NotepadForm::HandleAction(AppID nID, FindReplaceOption* findReplaceOption) {
	Action* action = ActionFactory::Create(this, nID, findReplaceOption);
	if (action != NULL)
	{
		action->Perform();

		if (action->NeedScrollBarUpdate())
		{
			this->Notify("UpdateScrollBars");
		}

		if (!action->ShouldKeepSelection())
		{
			this->note->Select(false);
			this->pagingBuffer->UnmarkSelectionBegin();
		}
		delete action;
	}

	this->Notify("UpdateStatusBar");
	this->Notify("CreateScrollBars");
	this->Notify("AdjustScrollBars");
	this->Notify("ChangeCaret");
	this->Invalidate();
}
#if 0

void NotepadForm::HandleMouseEvent(UINT nID, UINT nFlags, CPoint point, short zDelta) {
	CoordinateConverter coordinateConverter(this);
	CPoint absolutePoint = coordinateConverter.DisplayToAbsolute(point);
	
	MouseActionFactory mouseActionFactory(this);
	MouseAction* mouseAction = mouseActionFactory.Create(nID, nFlags, absolutePoint, zDelta);
	if (mouseAction != NULL)
	{
		mouseAction->Perform();
		if (mouseAction->NeedUpdateLatest())
		{
			this->mouseHandler->UpdateLatestPoint(absolutePoint);
		}

		if (!mouseAction->ShouldKeepSelection())
		{
			this->note->Select(false);
			this->pagingBuffer->UnmarkSelectionBegin();
		}
		delete mouseAction;
	}

	this->Notify("UpdateScrollBars");
	this->Notify("UpdateStatusBar");
	this->parent->Invalidate();
}
#endif




