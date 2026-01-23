#ifndef _PRINTERRESOURCECALCULATOR_H //guard
#define _PRINTERRESOURCECALCULATOR_H
#include <afxwin.h>
#include <afxdlgs.h>
#include "PageSetting.h"

typedef signed long int Long;

class PrinterResourceCalculator {
public:
	PrinterResourceCalculator(CWnd* parent, CPrintDialog* printDialog);
	~PrinterResourceCalculator();

	void Calculate();

	Long GetDpiX() const;
	Long GetDpiY() const;
	Long GetPhysicalWidth() const;
	Long GetPhysicalHeight() const;
	Margin& GetPixelMargin() const;
	CFont* GetFont() const;
	Long GetRowHeight() const;
private:
	CWnd* parent;
	CPrintDialog* printDialog;
	Long dpiX;
	Long dpiY;
	Long physicalWidth;
	Long physicalHeight;
	Margin pixelMargin;
	CFont* font;
	Long rowHeight;
};

inline Long PrinterResourceCalculator::GetDpiX() const {
	return this->dpiX;
}

inline Long PrinterResourceCalculator::GetDpiY() const {
	return this->dpiY;
}

inline Long PrinterResourceCalculator::GetPhysicalWidth() const {
	return this->physicalWidth;
}

inline Long PrinterResourceCalculator::GetPhysicalHeight() const {
	return this->physicalHeight;
}

inline Margin& PrinterResourceCalculator::GetPixelMargin() const {
	return const_cast<Margin&>(this->pixelMargin);
}

inline CFont* PrinterResourceCalculator::GetFont() const {
	return const_cast<CFont*>(this->font);
}

inline Long PrinterResourceCalculator::GetRowHeight() const {
	return this->rowHeight;
}

#endif // !_PRINTERRESOURCECALCULATOR_H
