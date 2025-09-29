#ifndef _MULTIBYTEWRAPPINGPOLICY_H
#define _MULTIBYTEWRAPPINGPOLICY_H
#include "AutoWrappingPolicy.h"

typedef signed long int Long;

class Glyph;

class MultiByteWrappingPolicy : public AutoWrappingPolicy {
public:
	MultiByteWrappingPolicy();
	virtual ~MultiByteWrappingPolicy();

	virtual Long FindWrappingPoint(Glyph* row, Long overflowedIndex);
};

#endif // !_MULTIBYTEWRAPPINGPOLICY_H
