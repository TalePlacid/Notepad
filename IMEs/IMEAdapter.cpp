#include "IMEAdapter.h"
#include <imm.h>
#include "IMEController.h"
#include "IMEResolver.h"
#include "../message.h"
#include "../TimerConstants.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../WritingModeSelector.h"

#pragma warning(disable:4996)

IMEAdapter::IMEAdapter(CWnd* parent) {
	this->parent = parent;
	this->imeController = new IMEController(this->parent->GetSafeHwnd(),
		((NotepadForm*)(this->parent))->caretController);
	this->isWaitingForComposition = FALSE;
	this->isWaitingForConversion = FALSE;
	this->source[0] = '\0';
	this->source[1] = '\0';
	this->converted[0] = '\0';
	this->converted[1] = '\0';
}

IMEAdapter::~IMEAdapter() {
	if (this->imeController != NULL)
	{
		delete this->imeController;
	}
}

void IMEAdapter::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam,
	AppID& appID, BOOL& isProceed, TCHAR(*character), BOOL& onChar) {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	appID = AppID::NONE;
	isProceed = FALSE;
	onChar = FALSE;

	TCHAR compositionString[256];
	Long compositionCharacterLength;
	this->imeController->GetCurrentCompositionText(compositionString, compositionCharacterLength);

	ImeID ImeId = IMEResolver::Resolve(message, wParam, lParam,
		this->isWaitingForComposition, this->isWaitingForConversion, compositionCharacterLength);
	switch (ImeId)
	{
	case ImeID::ID_IME_WRITE_COMPOSITION_CHARACTER:
		character[0] = compositionString[0];
		character[1] = compositionString[1];
		appID = WritingModeSelector::DetermineWritingMode(pagingBuffer);
		onChar = FALSE;
		isProceed = TRUE;
		break;
	case ImeID::ID_IME_WRTIE_COMPLETE_CHARACTER:
		character[0] = (BYTE)(wParam >> 8);
		character[1] = (BYTE)wParam;
		appID = WritingModeSelector::DetermineWritingMode(pagingBuffer);
		onChar = TRUE;
		isProceed = TRUE;
		break;
	case ImeID::ID_IME_ERASE_COMPOSITION_CHARACTER:
		appID = AppID::ID_COMMAND_ERASE_BEFORE_CARET;
		onChar = FALSE;
		isProceed = TRUE;
		break;
	case ImeID::ID_IME_SET_COMPOSITION_OPTION:
		this->BeginWaitingForConversion();
		this->parent->PostMessage(WM_IME_CONVERSION, 0, 0);
		isProceed = TRUE;
		break;
	case ImeID::ID_IME_CONVERSION:
		this->imeController->SetWindowPosition();
		this->imeController->Convert();
		this->parent->SetTimer(TIMER_ID_IME_OPEN_CANDIDATE, IME_OPEN_CANDIDATE_INTERVAL, 0);
		isProceed = TRUE;
		break;
	case ImeID::ID_IME_OPEN_CANDIDATE:
		this->parent->KillTimer(TIMER_ID_IME_OPEN_CANDIDATE);
		break;
	case ImeID::ID_IME_CLOSE_CANDIDATE:
		this->parent->SetTimer(TIMER_ID_IME_CLOSE_CANDIDATE, IME_CLOSE_CANDIDATE_INTERVAL, 0);
		break;
	case ImeID::ID_IME_CONVERT_CHARACTER:
		this->parent->KillTimer(TIMER_ID_IME_CLOSE_CANDIDATE);
		character[0] = (BYTE)(wParam >> 8);
		character[1] = (BYTE)wParam;
		this->RecordConverted(character);
		if (this->IsConverted())
		{
			appID = AppID::ID_COMMAND_CONVERT_TO_IME_CHARACTER;
		}
		this->ClearCharacters();
		this->EndWaitingForConversion();
		isProceed = TRUE;
		break;
	default:
		break;
	}
}

BOOL IMEAdapter::ApplyConversionOptions(const char(*character)) {
	return this->imeController->SetConversionOptions(character);
}

BOOL IMEAdapter::CloseCompositionWindow() {
	return this->imeController->CloseCompositionWindow();
}

char* IMEAdapter::RecordSource(char(*source)) {
	this->source[0] = source[0];
	this->source[1] = source[1];

	return this->source;
}

char* IMEAdapter::RecordConverted(char(*converted)) {
	this->converted[0] = converted[0];
	this->converted[1] = converted[1];

	return this->converted;
}

void IMEAdapter::ClearCharacters() {
	this->source[0] = '\0';
	this->source[1] = '\0';
	this->converted[0] = '\0';
	this->converted[1] = '\0';
}

BOOL IMEAdapter::IsConverted() const {
	BOOL isConverted = FALSE;

	if (this->converted[0] != '\0'
		&& (this->source[0] != this->converted[0]
			|| this->source[1] != this->converted[1]))
	{
		isConverted = TRUE;
	}

	return isConverted;
}
