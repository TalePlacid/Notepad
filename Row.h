#ifndef _ROW_H //guard
#define _ROW_H
#include "Composite.h"
#include "Array.h"

typedef signed long int Long;

class Glyph;
class TextOutVisitor;

class Row : public Composite {
public:
	Row(Long capacity = 128);
	virtual ~Row();
	Row(const Row& source);
	Row& operator=(const Row& source);

	virtual string MakeString();
	virtual Glyph* Clone();
	virtual void Accept(TextOutVisitor& textOutVisitor);
};

#endif // !_ROW_H