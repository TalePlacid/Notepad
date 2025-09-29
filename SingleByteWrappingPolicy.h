#ifndef _SINGLEBYTEWRAPPINGPOLICY_H //guard
#define _SINGLEBYTEWRAPPINGPOLICY_H
#include "AutoWrappingPolicy.h"

typedef signed long int Long;

class Glyph;

class SingleByteWrappingPolicy : public AutoWrappingPolicy {
public:
	SingleByteWrappingPolicy();
	virtual ~SingleByteWrappingPolicy();

	virtual Long FindWrappingPoint(Glyph* row, Long overflowedIndex);
};

#endif // !_SINGLEBYTEWRAPPINGPOLICY_H
