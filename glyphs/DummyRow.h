#ifndef _DUMMYROW_H //guard
#define _DUMMYROW_H
#include "Row.h"

typedef signed long int Long;

class Glyph;

class DummyRow : public Row {
public:
	DummyRow(Long capacity = 128, bool isDirty = true);
	virtual ~DummyRow();
	DummyRow(const DummyRow& source);
	DummyRow& operator=(const DummyRow& source);

	virtual Glyph* Clone();
	virtual bool IsDummyRow();
};

#endif // !_DUMMYROW_H
