#ifndef _TABSTOPCALCULATOR_H //guard
#define _TABSTOPCALCULATOR_H

typedef signed long int Long;

class TabStopCalculator {
public:
	TabStopCalculator();
	~TabStopCalculator();

	static Long CalculateNext(Long logicalX, Long averageCharacterWidth);
	static Long CalculateMarginToNext(Long logicalX, Long averageCharacterWidth);
};

#endif // !_TAPSTOPCALCULATOR_H
