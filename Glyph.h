#ifndef _GLYPH_H //guard
#define _GLYPH_H
#include <string>
using namespace std;

typedef signed long int Long;

class Visitor;

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
	virtual bool IsWordCharacter();
	virtual bool IsMultiByteCharacter();
	virtual void Accept(Visitor& visitor);
	virtual bool Select(bool isSelecting);

	virtual Glyph* operator[](Long index);
	virtual operator char*() const;

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetCurrent() const;
	virtual bool IsSelected() const;
protected:
	bool isSelected;
};

inline Long Glyph::GetCapacity() const {
	return 0;
}

inline Long Glyph::GetLength() const {
	return 0;
}

inline Long Glyph::GetCurrent() const {
	return -1;
}

inline bool Glyph::IsSelected() const {
	return this->isSelected;
}

#endif // !_GLYPH_H
