#ifndef _AUTOWRAPPINGPOLICY_H //guard
#define _AUTOWRAPPINGPOLICY_H

typedef signed long int Long;

class Glyph;

class AutoWrappingPolicy {
public:
	AutoWrappingPolicy();
	virtual ~AutoWrappingPolicy() = 0;

	virtual Long FindWrappingPoint(Glyph* row, Long overflowedIndex) = 0;
};

#endif // !_AUTOWRAPPINGPOLICY_H
