#ifndef _ROW_H //guard
#define _ROW_H
#include "Composite.h"
#include "Array.h"

typedef signed long int Long;

class Glyph;
class Visitor;

class Row : public Composite {
public:
	Row(Long capacity = 512, bool isDirty = true);
	virtual ~Row();
	Row(const Row& source);
	Row& operator=(const Row& source);

	virtual string MakeString();
	virtual Glyph* Clone();
	virtual void Accept(Visitor& visitor);

	virtual Long FindSelectionStart();
	virtual Long FindSelectionEnd();

	static int CompareSelection(void* one, void* other);
};

#endif // !_ROW_H