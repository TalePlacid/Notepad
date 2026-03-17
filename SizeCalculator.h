#ifndef _SIZECALCULATOR_H //guard
#define _SIZECALCULATOR_H
#include "Observer.h"
#include <afxwin.h>

typedef signed long int Long;

class Glyph;
class MultiByteWidthCache;

class SizeCalculator{
public:
	SizeCalculator(CWnd* parent);
	~SizeCalculator();

	Long GetCharacterWidth(char(*character), Long logicalX);
	Long GetRowWidth(CString contents);
	Long GetRowWidth(Glyph* row, Long columnIndex);
	Long GetNearestColumnIndex(Glyph* row, Long width);

	Long GetAverageCharacterWidth() const;
	Long GetRowHeight() const;
private:
	CWnd* parent;
	Long(*singleByteWidths);
	MultiByteWidthCache* multiByteWidthCache;
	Long averageCharacterWidth;
	Long rowHeight;
};

inline Long SizeCalculator::GetAverageCharacterWidth() const {
	return this->averageCharacterWidth;
}

inline Long SizeCalculator::GetRowHeight() const {
	return this->rowHeight;
}

#endif // !_SIZECALCULATOR_H

