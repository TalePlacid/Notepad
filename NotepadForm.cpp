#include <afxdlgs.h>
#include <imm.h>
#include <fstream>
#include <sstream>
#include "NotepadForm.h"
#include "Note.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "NoteConverter.h"
#include "CaretController.h"
#include "Caret.h"
#include "SizeCalculator.h"
#include "resource.h"
#include "CommandFactory.h"
#include "Command.h"
#include "WriteAtEndCommand.h"
#include "InsertAtCaretCommand.h"
#include "EraseCommand.h"
#include "KeyAction.h"
#include "KeyActionFactory.h"
#include "NoteWrapper.h"
#include "ScrollController.h"
#include "ScrollBarActionFactory.h"
#include "ScrollBarAction.h"
#include "TextOutVisitor.h"
#include "ClipboardController.h"
#include "PagingBuffer.h"
#include "Observer.h"
#include "SearchResultController.h"
#include "message.h"
#include "FindReplaceCommandFactory.h"
#include "HistoryBook.h"
#include "PreviewForm.h"
#include "StatusBarController.h"

#pragma warning(disable:4996)
#pragma comment(lib, "imm32.lib")

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

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
	ON_COMMAND_RANGE(ID_MENU_NEW, ID_MENU_PREVIEW, OnCommandRequested)
	ON_COMMAND_RANGE(ID_COMMAND_ERASE, ID_COMMAND_LOADLAST, OnCommandRequested)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
	ON_MESSAGE(WM_FINDREPLACE_FOCUS, OnFindReplaceFocused)
	ON_WM_CLOSE()
	END_MESSAGE_MAP()


NotepadForm::NotepadForm(CWnd *parent, StatusBarController* statusBarController){
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
	this->hasFindReplaceDialog = FALSE;
	this->previewForm = NULL;
	this->statusBarController = statusBarController;
	this->isCompositing = FALSE;
	this->isAutoWrapped = FALSE;
	this->magnification = 1.0;

	TCHAR buffer[256];
	GetCurrentDirectory(256, buffer);
	CString path(buffer);
	this->path = path + CString("\\NoName.txt");
}

NotepadForm::~NotepadForm() {

}

BOOL NotepadForm::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style &= ~WS_HSCROLL;
	cs.style &= ~WS_VSCROLL;

	return CWnd::PreCreateWindow(cs);
}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	//±âº» ÆùÆ®(¸¼Àº °íµñ, 10pt, º¸Åë ½ºÅ¸ÀÏ)
	LOGFONT logFont = { 0, };
	CClientDC dc(this);
	logFont.lfHeight = -MulDiv(10, dc.GetDeviceCaps(LOGPIXELSY), 72);
	logFont.lfWeight = FW_NORMAL;
	logFont.lfCharSet = HANGUL_CHARSET;
	_tcscpy_s(logFont.lfFaceName, "¸¼Àº °íµñ");

	this->originalFont = new CFont;
	this->originalFont->CreateFontIndirectA(&logFont);

	double round = 0.5;
	if (logFont.lfHeight < 0)
	{
		round = -0.5;
	}
	logFont.lfHeight = logFont.lfHeight * this->magnification + round;
	this->displayFont = new CFont;
	this->displayFont->CreateFontIndirectA(&logFont);

	this->sizeCalculator = new SizeCalculator(this);
	this->pagingBuffer = new PagingBuffer(this);
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

	Margin margin;
	margin.left = 20;
	margin.right = 20;
	margin.up = 25;
	margin.down = 25;
	this->pageSetting = PageSetting(CString("A4"), TRUE, margin, CString(""), CString(""));

	this->nextIsLastOnSize = FALSE;
	
	this->Notify("UpdateScrollBars");

	return 0;
}

void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if ((nChar > 31 && nChar < 127) || nChar == 9 || nChar == 13)
	{
		char character[2];
		character[0] = nChar;
		if (character[0] == '\r')
		{
			character[1] = '\n';
		}

		Long rowIndex = this->note->GetCurrent();
		Glyph* row = this->note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Command* command;
		if (rowIndex >= this->note->GetLength() - 1 && columnIndex >= row->GetLength())
		{
			command = new WriteAtEndCommand(this, character);
		}
		else
		{
			command = new InsertAtCaretCommand(this, character);
		}

		if (command != NULL)
		{
			command->Execute();
			if (command->IsUndoable())
			{
				//command = this->undoHistoryBook->Bind(command);
				this->undoHistoryBook->Push(command);
				this->redoHistoryBook->Clear();
			}
			else
			{
				delete command;
			}                 
		}

		this->note->Select(false);
		this->pagingBuffer->UnmarkSelectionBegin();
	
		this->Notify("ChangeCaret");
		this->Notify("UpdateScrollBars");
		this->Notify("UpdateStatusBar");
		this->Invalidate();
	}
}

void NotepadForm::OnSize(UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);

	if (this->isAutoWrapped)
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
		
		Long rowIndex = this->note->GetCurrent();
		Glyph* row = this->note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();
		
		if (length > 0)
		{
			character[length] = '\0';

			if (rowIndex >= this->note->GetLength() - 1 && columnIndex >= row->GetLength())
			{
				command = new WriteAtEndCommand(this, character, FALSE);
			}
			else
			{
				command = new InsertAtCaretCommand(this, character, FALSE);
			}

			if (command != NULL)
			{
				command->Execute();
				delete command;
			}			
			this->isCompositing = TRUE;
		}
		else if (length == 0)
		{
			this->isCompositing = FALSE;
			row->Remove(columnIndex - 1);
		}

		this->note->Select(false);
		this->pagingBuffer->UnmarkSelectionBegin();
		this->Invalidate();

		ImmReleaseContext(this->GetSafeHwnd(), himc);
		this->Notify("UpdateScrollBars");
		this->Notify("UpdateStatusBar");
	}
	
	return DefWindowProc(WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam) {
	char character[3];
	character[0] = (BYTE)(wParam >> 8);
	character[1] = (BYTE)wParam;
	character[2] = '\0';

	Long rowIndex = this->note->GetCurrent();
	Glyph* row = this->note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	Command* command;
	if (rowIndex >= this->note->GetLength() - 1 && columnIndex >= row->GetLength())
	{
		command = new WriteAtEndCommand(this, character);
	}
	else
	{
		command = new InsertAtCaretCommand(this, character);
	}

	if (command != NULL)
	{
		command->Execute();
		if (command->IsUndoable())
		{
			//command = this->undoHistoryBook->Bind(command);
			this->undoHistoryBook->Push(command);
			this->redoHistoryBook->Clear();
		}
		else
		{
			delete command;
		}
	}

	this->note->Select(false);
	this->pagingBuffer->UnmarkSelectionBegin();
	this->Invalidate();
	this->Notify("UpdateScrollBars");
	this->Notify("UpdateStatusBar");

	return 0;
}

LRESULT NotepadForm::OnImeEndComposition(WPARAM wParam, LPARAM lParam) {
	this->isCompositing = FALSE;

	return TRUE;
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

void NotepadForm::OnCommandRequested(UINT nID) {
	CommandFactory commandFactory;

	Command* command = commandFactory.Create(this, nID);
	if (command != NULL)
	{
		command->Execute();

		if (command->NeedScrollBarUpdate())
		{
			this->Notify("UpdateScrollBars");
		}

		if (command->IsUndoable())
		{
			//command = this->undoHistoryBook->Bind(command);
			this->undoHistoryBook->Push(command);
			this->redoHistoryBook->Clear();
		}
		else
		{
			delete command;
		}
	}

	this->Notify("UpdateStatusBar"); 
	this->Invalidate();
}

void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyActionFactory keyActionFactory;
	KeyAction* keyAction = keyActionFactory.Create(this, nChar);
	if (keyAction != NULL)
	{
		keyAction->Perform();
		
		if (keyAction->NeedScrollBarUpdate())
		{
			this->Notify("UpdateScrollBars");
		}
		
		if (!keyAction->ShouldKeepSelection())
		{
			this->note->Select(false);
			this->pagingBuffer->UnmarkSelectionBegin();
		}
		delete keyAction;
	}

	this->Notify("UpdateStatusBar"); 
	this->Invalidate();
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
	
	FindReplaceCommandFactory findReplaceCommandFactory;
	Command* command = findReplaceCommandFactory.Create(this, findReplaceDialog);
	if (command != NULL)
	{
		command->Execute();
		if (command->IsUndoable())
		{
			this->undoHistoryBook->Push(command);
			this->redoHistoryBook->Clear();
		}
		else
		{
			delete command;
		}
	}

	this->Notify("UpdateScrollBars");
	this->Notify("UpdateStatusBar"); 
	this->Invalidate();

	return 0;
}

LRESULT NotepadForm::OnFindReplaceFocused(WPARAM wParam, LPARAM lParam) {
	CFindReplaceDialog* findReplaceForm = (CFindReplaceDialog*)wParam;
	findReplaceForm->SetFocus();

	return 0;
}

void NotepadForm::OnClose() {
	if (this->note != NULL)
	{
		TCHAR buffer[256];
		GetCurrentDirectory(256, buffer);
		CString path(buffer);
		this->Save(path);

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

	CWnd::OnClose();
}

void NotepadForm::Load(CString path, TCHAR*(*str), Long& count) {
	(*str) = NULL;
	count = 0;

	FILE* file;
	file = fopen((LPCTSTR)path, "rb+");
	if (file != NULL)
	{
		fseek(file, 0, SEEK_END);
		Long fileEndOffset = ftell(file);
		(*str) = new TCHAR[fileEndOffset + 1];
		fseek(file, 0, SEEK_SET);
		count = fread((*str), 1, fileEndOffset, file);
		(*str)[count] = '\0';
		fclose(file);
	}
#if 0
	ifstream ifs;
	ifs.open(path);

	CString str("");
	if (ifs.is_open())
	{
		stringstream ss;
		ss << ifs.rdbuf();
		str.Format("%s", ss.str().c_str());
		str.Replace("\n", "\r\n");

		this->path = path;
		ifs.close();
	}

	return str;
#endif
}

void NotepadForm::Save(CString path) {
	CString tempFile = path + CString("\\Note.tmp");

	ifstream ifs;
	ifs.open(tempFile);

	CString noName = path + CString("\\NoName.txt");

	ofstream ofs;
	ofs.open(noName, ios::out | ios::binary);
	if (ifs.is_open() && ofs.is_open())
	{
		ofs << ifs.rdbuf();
		ifs.close();
		ofs.close();
	}
}