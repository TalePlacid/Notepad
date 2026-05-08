#ifndef _IMECONTROLLER_H //guard
#define _IMECONTROLLER_H
#include <imm.h>

typedef signed long int Long;

class CWnd;

class IMEController {
public:
	IMEController(CWnd* parent);
	~IMEController();

	bool SetConversionOptions(const char* korean);
	bool SetWindowPosition();
	bool Convert();
	void GetCurrentCompositionText(char* text, Long& length);

	char* RecordSource(char(*source));
	char* RecordConverted(char(*converted));
	void ClearCharacters();
	bool IsConverted();

	char* GetConverted();
private:
	CWnd* parent;
	HIMC hIMC;
	char source[2];
	char converted[2];
};

inline char* IMEController::GetConverted() {
	return this->converted;
}

#endif // !_IMECONTROLLER_H
