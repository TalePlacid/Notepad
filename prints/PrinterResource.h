#ifndef _PRINTERRESOURCE_H //guard
#define _PRINTERRESOURCE_H
#include "PageSetting.h"

typedef signed long int Long;

class CWnd;
class CPrintDialog;

class PrinterResource {
public:
	PrinterResource(CWnd* parent, CPrintDialog* printDialog);
	~PrinterResource();

	void LoadMetrics();

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

inline Long PrinterResource::GetDpiX() const {
	return this->dpiX;
}

inline Long PrinterResource::GetDpiY() const {
	return this->dpiY;
}

inline Long PrinterResource::GetPhysicalWidth() const {
	return this->physicalWidth;
}

inline Long PrinterResource::GetPhysicalHeight() const {
	return this->physicalHeight;
}

inline Margin& PrinterResource::GetPixelMargin() const {
	return const_cast<Margin&>(this->pixelMargin);
}

inline CFont* PrinterResource::GetFont() const {
	return const_cast<CFont*>(this->font);
}

inline Long PrinterResource::GetRowHeight() const {
	return this->rowHeight;
}

#endif // !_PRINTERRESOURCE_H

