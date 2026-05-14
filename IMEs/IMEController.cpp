#include <afxwin.h>
#include "IMEController.h"
#include "../CaretController.h"
#include "../Caret.h"

#pragma warning(disable:4996)

IMEController::IMEController(HWND hWnd, CaretController* caretController) {
    this->hWnd = hWnd;
    this->caretController = caretController;
    this->hIMC = ::ImmGetContext(this->hWnd);
}

IMEController::~IMEController() {
    ::ImmReleaseContext(this->hWnd, this->hIMC);
}

bool IMEController::SetConversionOptions(const char* character) {
    //1. 변환 문자 정보를 IME에 주입한다.
    BYTE attrs[2] = { ATTR_TARGET_NOTCONVERTED , ATTR_TARGET_NOTCONVERTED };
    DWORD clauses[2] = { 0, 2 };

    ImmSetCompositionStringA(this->hIMC, SCS_SETSTR, (LPVOID)character, 2, (LPVOID)character, 2);
    ImmSetCompositionStringA(this->hIMC, SCS_CHANGEATTR, attrs, 2, attrs, 2);
    ImmSetCompositionStringA(this->hIMC, SCS_CHANGECLAUSE, clauses, sizeof(clauses), clauses, sizeof(clauses));

    //2. WM_IME_COMPOSITION을 발생시켜 적용시킨다.
    BOOL isSucceed = ImmNotifyIME(this->hIMC, NI_OPENCANDIDATE, 0, 0);

    return isSucceed;
}

bool IMEController::SetWindowPosition() {
    //1. IME 조합창을 사용자에게 보이지 않도록 화면 밖으로 보낸다.
    COMPOSITIONFORM compositionForm;
    compositionForm.dwStyle = CFS_FORCE_POSITION;
    compositionForm.ptCurrentPos.x = -32000;
    compositionForm.ptCurrentPos.y = -32000;
    compositionForm.rcArea.left = -32000;
    compositionForm.rcArea.top = -32000;
    compositionForm.rcArea.right = -31999;
    compositionForm.rcArea.bottom = -31999;
    BOOL isCompositionSetted = ImmSetCompositionWindow(this->hIMC, &compositionForm);

    //2. 한자 후보창을 현재 캐럿 위치를 기준으로 띄운다.
    Caret* caret = this->caretController->GetCaret();
    POINT caretPoint;
    caretPoint.x = caret->GetX();
    caretPoint.y = caret->GetY();

    CANDIDATEFORM candidateForm;
    candidateForm.dwIndex = 0;
    candidateForm.dwStyle = CFS_EXCLUDE;
    candidateForm.ptCurrentPos = caretPoint;
    candidateForm.rcArea.left = caretPoint.x;
    candidateForm.rcArea.top = caretPoint.y;
    candidateForm.rcArea.right = caretPoint.x + caret->GetWidth();
    candidateForm.rcArea.bottom = caretPoint.y + caret->GetHeight();
    BOOL isCandidateSetted = ImmSetCandidateWindow(this->hIMC, &candidateForm);

    bool isSucceed = false;
    if (isCompositionSetted && isCandidateSetted)
    {
        isSucceed = true;
    }

    return isSucceed;
}

bool IMEController::Convert() {
    //1. 설정 완료된 상태에서 한자 변환키을 요청한다.
    BOOL isConvertKeyDown = ImmSimulateHotKey(this->hWnd, IME_KHOTKEY_HANJACONVERT);

    //2. IME입력기에 요청을 적용한다.
    BOOL isCandidateOpened = ImmNotifyIME(this->hIMC, NI_OPENCANDIDATE, 0, 0);

    bool isSucceed = false;
    if (isConvertKeyDown && isCandidateOpened)
    {
        isSucceed = true;
    }

    return isSucceed;
}

bool IMEController::CloseCompositionWindow() {
    return ImmNotifyIME(this->hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
}

void IMEController::GetCurrentCompositionText(char* text, Long& length) {
    length = ImmGetCompositionString(this->hIMC, GCS_COMPSTR, text, 256);
    if (length < 0)
    {
        length = 0;
    }
    text[length] = '\0';
}
