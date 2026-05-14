#ifndef _IMECONTROLLER_H //guard
#define _IMECONTROLLER_H
#include <afxwin.h>
#include <imm.h>

typedef signed long int Long;

class CaretController;

class IMEController {
public:
	IMEController(HWND hWnd, CaretController* caretController);
	~IMEController();

	bool SetConversionOptions(const char* korean);
	bool SetWindowPosition();
	bool Convert();
	void GetCurrentCompositionText(char* text, Long& length);
	bool CloseCompositionWindow();
private:
	HWND hWnd;
	HIMC hIMC;
	CaretController* caretController;
};

#endif // !_IMECONTROLLER_H
