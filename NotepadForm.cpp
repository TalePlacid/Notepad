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
#include "FontSelector.h"

#include "NoteConverter.h"
#include "NoteWrapper.h"
#include "PaintingVisitor.h"
#include "EncodingDetector.h"
#include "TextEncoder.h"
#include "CoordinateConverter.h"
#include "AutoScroller.h"
#include "WritingModeSelector.h"
#include "KeyDownInterpreter.h"
#include "MenuInterpreter.h"
#include "FindReplaceInterpreter.h"
#include "ScrollBarInterpreter.h"
#include "FindReplaceOptionMaker.h"
#include "FindReplaceRequestResolver.h"
#include "MouseEventResolver.h"

#include "glyphs/GlyphFactory.h"
#include "commands/CommandFactory.h"
#include "actions/ActionFactory.h"

#include "prints/PreviewForm.h"
#include "Caret.h"
#include "glyphs/Glyph.h"
#include "commands/Command.h"
#include "actions/Action.h"

#pragma warning(disable:4996)
#pragma comment(lib, "imm32.lib")

static const UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);
static const UINT_PTR TIMER_ID_RESIZE_REFRESH = 1;
static const UINT_PTR TIMER_ID_AUTO_SCROLL = 2;
static const UINT RESIZE_REFRESH_INTERVAL = 75;
static const UINT AUTO_SCROLL_INTERVAL = 30;

BEGIN_MESSAGE_MAP(NotepadForm, CWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_SIZE()
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
	this->originalFont = NULL;
	this->displayFont = NULL;
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
	this->isAutoWrapped = FALSE;
	this->autoWrapSuspendCount = 0;
	this->magnification = 1.0;
	this->isDirty = FALSE;
	this->encoding = ANSI;
	this->clientAreaSize.width = 0;
	this->clientAreaSize.height = 0;

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
		delete this->originalFont;
	}

	if (this->displayFont != NULL)
	{
		delete this->displayFont;
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

	CRect clientArea;
	this->GetClientRect(&clientArea);
	this->UpdateClientAreaSize(clientArea.Width(), clientArea.Height());
	
	FontSelector fontSelector;
	LOGFONT logFont = fontSelector.SelectBaseLogFont(this);

	this->originalFont = new CFont;
	this->originalFont->CreateFontIndirect(&logFont);

	logFont = fontSelector.SelectScaledLogFont(logFont, this->magnification);
	this->displayFont = new CFont;
	this->displayFont->CreateFontIndirect(&logFont);

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
	if (this->statusBarController != NULL)
	{
		this->Register(this->statusBarController);
	}

	this->mouseHandler = new MouseHandler(this);

	Margin margin;
	margin.left = 20;
	margin.right = 20;
	margin.up = 25;
	margin.down = 25;
	this->pageSetting = PageSetting(CString("A4"), TRUE, margin, CString(""), CString(""));

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
			this->HandleCommand(AppID::ID_COMMAND_ERASE_BEFORE_CARET, NULL, FALSE);
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

	if (cx > 0 && cy > 0)
	{
		this->UpdateClientAreaSize(cx, cy);
		KillTimer(TIMER_ID_RESIZE_REFRESH);
		SetTimer(TIMER_ID_RESIZE_REFRESH, RESIZE_REFRESH_INTERVAL, NULL);
	}
}

void NotepadForm::OnPaint() {
	CPaintDC dc(this);

	ClientAreaSize clientAreaSize = this->GetClientAreaSize();
	Long width = clientAreaSize.width;
	Long height = clientAreaSize.height;
	
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(&dc, width, height);

	CBitmap* oldBitMap = memDC.SelectObject(&bitMap);

	memDC.FillSolidRect(&rect, RGB(255, 255, 255));
	PaintingVisitor paintingVisitor(this, &memDC);
	this->note->Accept(paintingVisitor);

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
	AppID nID = ScrollBarInterpreter::DetermineID(SB_VERT, nSBCode);
	if (nID != AppID::NONE)
	{
		this->HandleAction(nID);
	}
}

void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	AppID nID = ScrollBarInterpreter::DetermineID(SB_HORZ, nSBCode);
	if (nID != AppID::NONE)
	{
		this->HandleAction(nID);
	}
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

void NotepadForm::OnLButtonDown(UINT nFlags, CPoint point) {
	this->ResolveMouseEvent(AppID::ID_MOUSE_LBUTTON_DOWN, nFlags, point);
}

void NotepadForm::OnMouseMove(UINT nFlags, CPoint point) {
	if (nFlags == MK_LBUTTON && this->mouseHandler->OnDrag())
	{
		AutoScroller autoScroller(this);
		BOOL isAutoScrolled = autoScroller.ScrollIfNeeded(point);
		if (isAutoScrolled)
		{
			SetTimer(TIMER_ID_AUTO_SCROLL, AUTO_SCROLL_INTERVAL, NULL);
		}

		this->ResolveMouseEvent(AppID::ID_MOUSE_MOVE, nFlags, point);
	}
}

void NotepadForm::OnLButtonUp(UINT nFlags, CPoint point) {
	this->ResolveMouseEvent(AppID::ID_MOUSE_LBUTTON_UP, nFlags, point);
	KillTimer(TIMER_ID_AUTO_SCROLL);
}

BOOL NotepadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	CPoint clientPoint = pt;
	this->ScreenToClient(&clientPoint);
	this->ResolveMouseEvent(AppID::ID_MOUSE_WHEEL, nFlags, clientPoint, zDelta);
	return 0;
}

void NotepadForm::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent)
	{
	case TIMER_ID_RESIZE_REFRESH:
	{
		KillTimer(TIMER_ID_RESIZE_REFRESH);
		if (this->isAutoWrapped)
		{
			SizeCalculator* sizeCalculator = this->sizeCalculator;
			ScrollController* scrollController = this->scrollController;
			Scroll vScroll = scrollController->GetVScroll();
			Long max = vScroll.GetMax();
			Long rowHeight = sizeCalculator->GetRowHeight();

			NoteWrapper noteWrapper(this);
			Long unwrappedCount = noteWrapper.Unwrap();
			Long wrappedCount = noteWrapper.Wrap();
			max += (wrappedCount - unwrappedCount) * rowHeight;
			scrollController->ResizeVRange(max);
		}
		this->Notify("UpdateScrollBars");
		this->Invalidate();
	}
	break;
	case TIMER_ID_AUTO_SCROLL:
	{
		CPoint cursorPos;
		GetCursorPos(&cursorPos);
		this->ScreenToClient(&cursorPos);
		AutoScroller autoScroller(this);
		autoScroller.ScrollIfNeeded(cursorPos);

		this->ResolveMouseEvent(AppID::ID_MOUSE_MOVE, MK_LBUTTON, cursorPos);
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

void NotepadForm::ResolveMouseEvent(AppID rawID, UINT nFlags, CPoint point, short zDelta) {
	point = MouseEventResolver::NormalizePoint(point, this->clientAreaSize);
	AppID appID = MouseEventResolver::Resolve(rawID, nFlags, point, this->mouseHandler->OnDrag(), zDelta);
	CoordinateConverter coordinateConverter(this);
	CPoint absolutePoint = coordinateConverter.DisplayToAbsolute(point);

	this->HandleAction(appID, NULL, &absolutePoint);
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
			this->isDirty = TRUE;
		}
		else
		{
			delete command;
		}
	}

	this->isDirty = TRUE;
	this->Notify("UpdateStatusBar");
	this->Invalidate();
}

void NotepadForm::HandleAction(AppID nID, FindReplaceOption* findReplaceOption,
	CPoint* point, short zDelta) {
	Action* action = ActionFactory::Create(this, nID, findReplaceOption, point, zDelta);
	if (action != NULL)
	{
		action->Perform();

		if (action->NeedScrollBarUpdate())
		{
			this->Notify("UpdateScrollBars");
		}

		if (action->NeedUpdateLatestPoint())
		{
			this->mouseHandler->UpdateLatestPoint(*point);
		}

		if (!action->ShouldKeepSelection())
		{
			this->note->Select(false);
			this->pagingBuffer->UnmarkSelectionBegin();
		}

		this->undoHistoryBook->StopBinding();

		delete action;
	}

	this->Notify("UpdateStatusBar");
	this->Notify("CreateScrollBars");
	this->Notify("AdjustScrollBars");
	this->Notify("ChangeCaret");
	this->Invalidate();
}
