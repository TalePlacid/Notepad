#ifndef _GLYPH_H //guard
#define _GLYPH_H
#include <string>
using namespace std;

typedef signed long int Long;

class Glyph {
public:
	Glyph();
	virtual ~Glyph() = 0;
	Glyph(const Glyph& source);
	Glyph& operator=(const Glyph& source);

	virtual Long Add(Glyph* glyph);
	virtual Long Add(Long index, Glyph* glyph);
	virtual Glyph* GetAt(Long index);
	virtual Glyph* Clone() = 0;
	virtual string MakeString() = 0;
	virtual Long Remove();
	virtual Long Remove(Long index);
	virtual Long First();
	virtual Long Previous();
	virtual Long Next();
	virtual Long Last();
	virtual Long Move(Long index);
	virtual bool IsDummyRow();

	virtual Glyph* operator[](Long index);

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetCurrent() const;
};

#endif // !_GLYPH_H
