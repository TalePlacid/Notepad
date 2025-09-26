#ifndef _SIZECALCULATOR_H //guard
#define _SIZECALCULATOR_H
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
	Long GetMaxCharacterWidth() const;
private:
	Long(*singleByteWidths);
	Long multiByteWidth;
	Long rowHeight;
	Long maxCharacterWidth;
};

inline Long SizeCalculator::GetMultiByteWidth() const {
	return this->multiByteWidth;
}

inline Long SizeCalculator::GetRowHeight() const {
	return this->rowHeight;
}

inline Long SizeCalculator::GetMaxCharacterWidth() const {
	return this->maxCharacterWidth;
}

#endif // !_SIZECALCULATOR_H
