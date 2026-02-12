#include <afxwin.h>
#include "NewWindowCommand.h"

#pragma warning(disable:4996)

NewWindowCommand::NewWindowCommand(CWnd* parent)
	:Command(parent) {

}

NewWindowCommand::~NewWindowCommand() {

}

void NewWindowCommand::Execute() {
    TCHAR exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    CString commandLine;
    commandLine.Format("\"%s\"", exePath);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL isSucceeded = CreateProcess(
        NULL,           
        commandLine.GetBuffer(),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    commandLine.ReleaseBuffer();
    if (isSucceeded)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}