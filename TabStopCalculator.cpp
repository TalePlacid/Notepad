#include "TabStopCalculator.h"

#pragma warning(disable:4996)

TabStopCalculator::TabStopCalculator() {
}

TabStopCalculator::~TabStopCalculator() {

}

Long TabStopCalculator::CalculateNext(Long logicalX, Long averageCharacterWidth) {
	Long tabSize = averageCharacterWidth * 8;

	Long tabStop = (logicalX / tabSize + 1) * tabSize;
	
	return tabStop;
}

Long TabStopCalculator::CalculateMarginToNext(Long logicalX, Long averageCharacterWidth) {
	Long tabStop = TabStopCalculator::CalculateNext(logicalX, averageCharacterWidth);
	Long margin = tabStop - logicalX;

	return margin;
}
