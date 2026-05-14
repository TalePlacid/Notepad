#include "IMEResolver.h"
#include <imm.h>
#include "../message.h"

#pragma warning(disable:4996)

IMEResolver::IMEResolver() {

}

IMEResolver::~IMEResolver() {

}

ImeID IMEResolver::Resolve(UINT message, WPARAM wParam, LPARAM lParam,
	BOOL isWaitingForComposition, BOOL isWaitingForConversion, Long compositionCharacterLength) {
	ImeID id = ImeID::NONE;
	switch (message)
	{
	case WM_IME_COMPOSITION:
		if (lParam & GCS_COMPSTR && !isWaitingForConversion) 
		{
			if (!isWaitingForComposition)
			{
				if (compositionCharacterLength > 0)
				{
					id = ImeID::ID_IME_WRITE_COMPOSITION_CHARACTER;
				}
				else
				{
					id = ImeID::ID_IME_ERASE_COMPOSITION_CHARACTER;
				}
			}
			else
			{
				id = ImeID::ID_IME_SET_COMPOSITION_OPTION;
			}
		}
		break;
	case WM_IME_CONVERSION:
		if (isWaitingForConversion)
		{
			id = ImeID::ID_IME_CONVERSION;
		}
		break;
	case WM_IME_NOTIFY:
		if (wParam == IMN_OPENCANDIDATE)
		{
			id = ImeID::ID_IME_OPEN_CANDIDATE;
		}
		else if (wParam == IMN_CLOSECANDIDATE)
		{
			id = ImeID::ID_IME_CLOSE_CANDIDATE;
		}
		break;
	case WM_IME_CHAR:
		if (!isWaitingForComposition)
		{
			if (!isWaitingForConversion)
			{
				id = ImeID::ID_IME_WRTIE_COMPLETE_CHARACTER;
			}
			else
			{
				id = ImeID::ID_IME_CONVERT_CHARACTER;
			}
		}
		break;
	default:
		break;
	}

	return id;
}