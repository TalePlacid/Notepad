#ifndef _COMPOSITE_H //guard
#define _COMPOSITE_H
#include "Glyph.h"
#include "Array.h"

typedef signed long int Long;

class TextOutVisitor;

class Composite : public Glyph {
public:
	Composite(Long capacity = 128);
	virtual ~Composite() = 0;
	Composite(const Composite& source);
	Composite& operator=(const Composite& source);

	virtual Long Add(Glyph* glyph);
	virtual Long Add(Long index, Glyph* glyph);
	virtual Glyph* GetAt(Long index);
	virtual Long Remove();
	virtual Long Remove(Long index);

	virtual Long First();
	virtual Long Previous();
	virtual Long Next();
	virtual Long Last();
	virtual Long Move(Long index);

	virtual void Accept(TextOutVisitor& textOutVisitor);

	virtual Glyph* operator[](Long index);

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetCurrent() const;
protected:
	Array<Glyph*> glyphs;
	Long capacity;
	Long length;
	Long current;
};

inline Long Composite::GetCapacity() const {
	return this->capacity;
}

inline Long Composite::GetLength() const {
	return this->length;
}

inline Long Composite::GetCurrent() const {
	return this->current;
}

#endif // !_COMPOSITE_H
