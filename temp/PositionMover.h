#ifndef _POSITIONMOVER_H //guard
#define _POSITIONMOVER_H
#include "Position.h"

typedef signed long int Long;

class PagingBuffer;

class PositionMover {
public:
	PositionMover(PagingBuffer* pagingBuffer);
	~PositionMover();

	Position First();
	Position Previous(Long oldCurrentOffset);
	Position Next(Long oldCurrentOffset);
	Position Last(Long oldCurrentOffset);
	Position Move(Long index);

	Position FirstRow();
	Position PreviousRow(Long oldCurrentOffset);
	Position NextRow(Long oldCurrentOffset);
	Position LastRow();
	Position MoveUpRows(Long oldCurrentOffset, Long count);
	Position MoveDownRows(Long oldCurrentOffset, Long count);
private:
	PagingBuffer* pagingBuffer;
};

#endif // !_POSITIONMOVER_H
