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
#include <imm.h>
#include <fstream>
#include <sstream>

#pragma warning(disable:4996)
#pragma comment(lib, "imm32.lib")

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
	ON_COMMAND_RANGE(ID_MENU_NEW, ID_COMMAND_ERASE, OnCommandRequested)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
	END_MESSAGE_MAP()


NotepadForm::NotepadForm() {
	this->note = NULL;
	this->isCompositing = FALSE;
	this->font = NULL;
	this->sizeCalculator = NULL;
	this->caretController = NULL;
	this->scrollBarController = NULL;

	TCHAR buffer[256];
	GetCurrentDirectory(256, buffer);
	CString path(buffer);
	this->path = path + CString("\\NoName.txt");
}

NotepadForm::~NotepadForm() {

}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);
	CString str = this->Load(this->path);
	this->note = new Note((LPCTSTR)str);
	//NoteConverter noteConverter;
	//this->note = noteConverter.ConvertToNote((LPCTSTR)str);
	this->sizeCalculator = new SizeCalculator(this);
	this->caretController = new CaretController(this);
	this->Register(this->caretController);
	
	this->scrollBarController = new ScrollBarController(this);
	this->Register(this->scrollBarController);

	this->isCompositing = FALSE;
	this->menu.LoadMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
	this->SetMenu(&(this->menu));

	this->Notify("CreateScrollBars");

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
		if (rowIndex != this->note->GetLength() - 1 || columnIndex != row->GetLength() - 1)
		{
			command = new InsertAtCaretCommand(this, &character);
		}
		else
		{
			command = new WriteAtEndCommand(this, &character);
		}

		if (command != NULL)
		{
			command->Execute();
			delete command;
		}

		this->Notify("CreateScrollBars");
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
}

void NotepadForm::OnPaint() {
	CPaintDC dc(this);
	TextOutVisitor textOutVisitor(this, &dc);

	this->note->Accept(textOutVisitor);

	this->Notify("ChangeCaret");
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

			if (rowIndex != this->note->GetLength() - 1 || columnIndex != row->GetLength() - 1)
			{
				command = new InsertAtCaretCommand(this, character);
			}
			else
			{
				command = new WriteAtEndCommand(this, character);
			}

			if (command != NULL)
			{
				command->Execute();
				delete command;
			}

			this->Notify("CreateScrollBars");
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
	if (rowIndex != this->note->GetLength() - 1 || columnIndex != row->GetLength() - 1)
	{
		command = new InsertAtCaretCommand(this, character);
	}
	else
	{
		command = new WriteAtEndCommand(this, character);
	}

	if (command != NULL)
	{
		command->Execute();
		delete command;
	}

	this->Invalidate();
	
	return DefWindowProc(WM_IME_CHAR, wParam, lParam);
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

void NotepadForm::OnClose() {
	if (this->note != NULL)
	{
		TCHAR buffer[256];
		GetCurrentDirectory(256, buffer);
		CString path(buffer);
		path += CString("\\NoName.txt");
		
		if (path == this->path)
		{
			this->Save(path);
		}
		delete this->note;
	}

	if (this->caretController != NULL)
	{
		delete this->caretController;
		this->caretController = NULL;
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
	ofstream ofs;
	ofs.open(path);
	if (ofs.is_open())
	{
		NoteWrapper noteWrapper(this);
		noteWrapper.DeleteDummyRows();
		CString str(this->note->MakeString().c_str());
		ofs << str;
		this->path = path;
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

WordState GetState(CString content) {
	WordState wordState;

	if (content[0] & 0x80)
	{
		wordState = VOCABLE;
	}
	else if ((content[0] >= 97 && content[0] <= 122) || (content[0] >= 65 && content[0] <= 90)
		|| (content[0] >= 48 && content[0] <= 57) || (content[0] == '_') || (content[0] == '\'') || (content[0] == '\"'))
		// 알파벳 소문자, 대문자, 숫자
	{
		wordState = VOCABLE;
	}
	else
	{
		wordState = BOUNDARY;
	}

	return wordState;
}