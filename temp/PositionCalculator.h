#ifndef _POSITIONCALCULATOR_H //guard
#define _POSITIONCALCULATOR_H
#include "Position.h"

typedef signed long int Long;

class PagingBuffer;

class PositionCalculator {
public:
	PositionCalculator(PagingBuffer* pagingBuffer);
	~PositionCalculator();

	Position First(Position position);
	Position Previous(Position position);
	Position Next(Position position, Long oldOffset);
	Position Last(Position position, Long oldOffset);
	Position Move(Position position, Long index);

	Position FirstRow();
	Position PreviousRow(Position position, Long oldOffset);
	Position NextRow(Position position, Long oldOffset);
	Position LastRow();
	Position MoveUpRows(Position position, Long oldOffset, Long count);
	Position MoveDownRows(Position position, Long oldOffset, Long count);
private:
	PagingBuffer* pagingBuffer;
};

#endif // !_POSITIONCALCULATOR_H
