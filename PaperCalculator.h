#ifndef _PAPERCALCULATOR_H //guard
#define _PAPERCALCULATOR_H
#include <afxwin.h>
#define CAPTION_SIZE 20

typedef signed long int Long;

class PaperCalculator {
public:
	PaperCalculator(CWnd* parent);
	~PaperCalculator();

	RECT CalculatePaperArea();
	RECT CalculateWritingArea(RECT paperArea);
private:
	CWnd* parent;
};

#endif // !_PAPERCALCULATOR_H
