#ifndef _SIZECALCULATOR_H //guard
#define _SIZE_CALCULATOR_H
#include "Observer.h"
#include <afxwin.h>

typedef signed long int Long;

class SizeCalculator{
public:
	SizeCalculator(CWnd* parent);
	~SizeCalculator();

	Long GetCharacterWidth(char(*character));

	Long GetMultiByteWidth() const;
	Long GetRowHeight() const;
private:
	Long(*singleByteWidths);
	Long multiByteWidth;
	Long rowHeight;
};

inline Long SizeCalculator::GetMultiByteWidth() const {
	return this->multiByteWidth;
}

inline Long SizeCalculator::GetRowHeight() const {
	return this->rowHeight;
}

#endif // !_SIZECALCULATOR_H
