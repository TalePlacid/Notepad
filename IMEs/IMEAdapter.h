#ifndef _IMEADAPTER_H //guard
#define _IMEADAPTER_H
#include <afxwin.h>
#include "../AppID.h"

class IMEController;

class IMEAdapter {
public:
	IMEAdapter(CWnd* parent);
	~IMEAdapter();

	void HandleMessage(UINT message, WPARAM wParam, LPARAM lParam,
		AppID& appID, BOOL& isProceed, TCHAR(*character), BOOL& onChar);
	BOOL ApplyConversionOptions(const char(*character));
	BOOL CloseCompositionWindow();

	BOOL BeginWaitingForComposition();
	BOOL EndWaitingForComposition();
	BOOL IsWaitingForComposition() const;
	BOOL BeginWaitingForConversion();
	BOOL EndWaitingForConversion();
	BOOL IsWaitingForConversion() const;
	
	char* RecordSource(char(*source));
	char* RecordConverted(char(*converted));
	void ClearCharacters();
	
	BOOL IsConverted() const;
	char* GetConverted();
private:
	CWnd* parent;
	IMEController* imeController;
	BOOL isWaitingForComposition;
	BOOL isWaitingForConversion;
	char source[2];
	char converted[2];
};

inline BOOL IMEAdapter::BeginWaitingForComposition() {
	this->isWaitingForComposition = TRUE;
	this->isWaitingForConversion = FALSE;

	return this->isWaitingForComposition;
}

inline BOOL IMEAdapter::EndWaitingForComposition() {
	this->isWaitingForComposition = FALSE;

	return this->isWaitingForComposition;
}

inline BOOL IMEAdapter::IsWaitingForComposition() const {
	return this->isWaitingForComposition;
}

inline BOOL IMEAdapter::BeginWaitingForConversion() {
	this->isWaitingForComposition = FALSE;
	this->isWaitingForConversion = TRUE;

	return this->isWaitingForConversion;
}

inline BOOL IMEAdapter::EndWaitingForConversion() {
	this->isWaitingForConversion = FALSE;

	return this->isWaitingForConversion;
}

inline BOOL IMEAdapter::IsWaitingForConversion() const {
	return this->isWaitingForConversion;
}

inline char* IMEAdapter::GetConverted() {
	return this->converted;
}

#endif // !_IMEADAPTER_H
