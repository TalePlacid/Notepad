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
#include "Font.h"
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
#include "ScrollBarController.h"
#include "ScrollBarActionFactory.h"
#include "ScrollBarAction.h"
#include "TextOutVisitor.h"
#include "ClipboardController.h"
#include "PagingBuffer.h"
#include "Observer.h"
#include "MarkingHelper.h"
#include "SearchResultController.h"
#include "message.h"
#include "FindReplaceCommandFactory.h"

#pragma warning(disable:4996)
#pragma comment(lib, "imm32.lib")

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
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
	ON_COMMAND_RANGE(ID_MENU_NEW, ID_COMMAND_CUT, OnCommandRequested)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
	ON_MESSAGE(WM_FINDREPLACE_CREATED, OnFindReplaceCreated)
	ON_WM_CLOSE()
	END_MESSAGE_MAP()


NotepadForm::NotepadForm() {
	this->note = NULL;
	this->isCompositing = FALSE;
	this->font = NULL;
	this->sizeCalculator = NULL;
	this->caretController = NULL;
	this->scrollBarController = NULL;
	this->clipboardController = NULL;
	this->pagingBuffer = NULL;
	this->searchResultController = NULL;
	this->hasFindReplaceForm = FALSE;

	TCHAR buffer[256];
	GetCurrentDirectory(256, buffer);
	CString path(buffer);
	this->path = path + CString("\\NoName.txt");
}

NotepadForm::~NotepadForm() {

}

BOOL NotepadForm::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style &= ~WS_HSCROLL;

	return CFrameWnd::PreCreateWindow(cs);
}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	this->sizeCalculator = new SizeCalculator(this);

	this->pagingBuffer = new PagingBuffer(this);
	this->pagingBuffer->Load();

	this->caretController = new CaretController(this);
	this->Register(this->caretController);
	this->caretController->Create();

	this->scrollBarController = new ScrollBarController(this);
	this->Register(this->scrollBarController);

	this->clipboardController = new ClipboardController(this);

	this->isCompositing = FALSE;
	this->menu.LoadMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
	this->SetMenu(&(this->menu));

	this->searchResultController = new SearchResultController;

	this->Notify("CreateScrollBars");
	this->Notify("AdjustScrollBars");

	return 0;
}

void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if ((nChar > 31 && nChar < 127) || nChar == 9 || nChar == 13)
	{
		char character = nChar;

		Long rowIndex = this->note->GetCurrent();
		Glyph* row = this->note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		Command* command;
		if (rowIndex >= this->note->GetLength() - 1 && columnIndex >= row->GetLength())
		{
			command = new WriteAtEndCommand(this, &character);
		}
		else
		{
			command = new InsertAtCaretCommand(this, &character);
		}

		if (command != NULL)
		{
			command->Execute();
			delete command;
		}

		this->note->Select(false);
		MarkingHelper markingHelper(this);
		markingHelper.Unmark();
	
		NoteWrapper noteWrapper(this);
		noteWrapper.DeleteDummyRows();

		UINT isChecked = this->menu.GetMenuState(ID_MENU_AUTOWRAP, MF_BYCOMMAND);
		if (isChecked == MF_CHECKED)
		{
			noteWrapper.InsertDummyRows();
		}

		this->Notify("ChangeCaret");
		this->Notify("CreateScrollBars");
		this->Notify("AdjustScrollBars");
		this->Invalidate();
	}
}

void NotepadForm::OnSize(UINT nType, int cx, int cy) {
	NoteWrapper noteWrapper(this);
	noteWrapper.DeleteDummyRows();

	UINT isChecked = this->menu.GetMenuState(ID_MENU_AUTOWRAP, MF_BYCOMMAND);
	if (isChecked == MF_CHECKED)
	{
		noteWrapper.InsertDummyRows();
	}

	this->Notify("CreateScrollBars");
	this->Notify("AdjustScrollBars");
}

void NotepadForm::OnPaint() {
	CPaintDC dc(this);

	RECT rect;
	this->GetClientRect(&rect);
	if (this->GetScrollPos(SB_HORZ) > 0)
	{
		rect.right += this->sizeCalculator->GetMultiByteWidth();
	}
	if (this->GetScrollPos(SB_VERT) > 0)
	{
		rect.bottom += this->sizeCalculator->GetRowHeight();
	}
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

	dc.BitBlt(0, 0, width, height, &memDC, textOutVisitor.GetXOffset(), textOutVisitor.GetYOffset(), SRCCOPY);

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
		if (length > 0)
		{
			character[length] = '\0';
	
			Long rowIndex = this->note->GetCurrent();
			Glyph* row = this->note->GetAt(rowIndex);
			Long columnIndex = row->GetCurrent();

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

			this->Notify("CreateScrollBars");
			this->Notify("AdjustScrollBars");
			this->isCompositing = TRUE;
		}
		else if (length == 0)
		{
			this->isCompositing = FALSE;
			command = new EraseCommand(this);
			if (command != NULL)
			{
				command->Execute();
				delete command;
			}
		}

		this->note->Select(false);
		MarkingHelper markingHelper(this);
		markingHelper.Unmark();
		this->Invalidate();

		ImmReleaseContext(this->GetSafeHwnd(), himc);
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
		delete command;
	}

	this->note->Select(false);
	MarkingHelper markingHelper(this);
	markingHelper.Unmark();
	this->Invalidate();
	
	//DefWindowProc(WM_IME_CHAR, wParam, lParam)
	return 0;
}

LRESULT NotepadForm::OnImeEndComposition(WPARAM wParam, LPARAM lParam) {
	this->isCompositing = FALSE;

	return TRUE;
}

void NotepadForm::OnSetFocus(CWnd* pOldWnd) {
	this->Notify("ChangeCaret");
}

void NotepadForm::OnKillFocus(CWnd* pNewWnd) {
	if (this->caretController != NULL)
	{
		this->caretController->Destroy();
	}

	if (this->note != NULL)
	{
		this->note->Select(false);
		MarkingHelper markingHelper(this);
		markingHelper.Unmark();
	}
}

void NotepadForm::OnCommandRequested(UINT nID) {
	CommandFactory commandFactory;

	Command* command = commandFactory.Create(this, nID);
	if (command != NULL)
	{
		command->Execute();
		delete command;
	}
	this->Notify("CreateScrollBars");
	this->Notify("AdjustScrollBars");
	this->Invalidate();
}

void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyActionFactory keyActionFactory;
	KeyAction* keyAction = keyActionFactory.Create(this, nChar);
	if (keyAction != NULL)
	{
		keyAction->Perform();
		delete keyAction;
	}
}

void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollBarActionFactory scrollBarActionFactory;
	ScrollBarAction* scrollBarAction = scrollBarActionFactory.Create(this, SB_VERT, nSBCode, nPos);
	if (scrollBarAction != NULL)
	{
		scrollBarAction->Perform();
		delete scrollBarAction;
	}
}

void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollBarActionFactory scrollBarActionFactory;
	ScrollBarAction* scrollBarAction = scrollBarActionFactory.Create(this, SB_HORZ, nSBCode, nPos);
	if (scrollBarAction != NULL)
	{
		scrollBarAction->Perform();
		delete scrollBarAction;
	}
}

BOOL NotepadForm::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}

LRESULT NotepadForm::OnFindReplace(WPARAM wParam, LPARAM lParam) {
	CFindReplaceDialog* findReplaceDialog = CFindReplaceDialog::GetNotifier(lParam);
	
	UINT id = 0;
	if (findReplaceDialog->FindNext())
	{
		if (CString(this->searchResultController->GetKey().c_str()) != findReplaceDialog->GetFindString())
		{	
			id = ID_COMMAND_FIND;
		}
		else
		{
			id = ID_COMMAND_FINDNEXT;
		}
	}

	FindReplaceCommandFactory findReplaceCommandFactory;
	Command* command = findReplaceCommandFactory.Create(this, findReplaceDialog, id);
	if (command != NULL)
	{
		command->Execute();
		delete command;
	}

	if (findReplaceDialog->IsTerminating())
	{
		this->hasFindReplaceForm = FALSE;
	}

	return 0;
}

LRESULT NotepadForm::OnFindReplaceCreated(WPARAM wParam, LPARAM lParam) {
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

	if (this->sizeCalculator != NULL)
	{
		delete this->sizeCalculator;
	}

	if (this->scrollBarController != NULL)
	{
		delete this->scrollBarController;
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

	CFrameWnd::OnClose();
}

CString NotepadForm::Load(CString path) {
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

class NotepadApp : public CWinApp {
public:
	NotepadApp();
	~NotepadApp();
	virtual BOOL InitInstance();
};

NotepadApp::NotepadApp() {

}

NotepadApp::~NotepadApp() {

}

BOOL NotepadApp::InitInstance() {
	NotepadForm *notepadForm = new NotepadForm;

	notepadForm->Create(NULL, "메모장 ~제목없음");

	notepadForm->ShowWindow(SW_SHOW);
	notepadForm->UpdateWindow();
	this->m_pMainWnd = notepadForm;

	return TRUE;
}

NotepadApp notepadApp;