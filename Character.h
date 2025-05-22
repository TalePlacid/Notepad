#ifndef _CHARACTER_H //guard
#define _CHARACTER_H
#include "Glyph.h"

class TextOutVisitor;

class Character : public Glyph {
public:
	Character();
	virtual ~Character() = 0;
	Character(const Character& source);
	Character& operator=(const Character& source);

	virtual void Accept(TextOutVisitor& textOutVisitor);
};

#endif // !_CHARACTER_H