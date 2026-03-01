#ifndef _COORDINATECONVERTER_H //guard
#define _COORDINATECONVERTER_H
#include <afxwin.h>

typedef signed long int Long;

class CoordinateConverter {
public:
	CoordinateConverter(CWnd* parent);
	~CoordinateConverter();

	CPoint DisplayToAbsolute(CPoint point);
	void AbsoluteToNotePosition(CPoint point, Long& rowIndex, Long& columnIndex);
private:
	CWnd* parent;
};

#endif // !_COORDINATECONVERTER_H

