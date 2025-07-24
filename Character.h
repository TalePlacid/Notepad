#ifndef _CHARACTER_H //guard
#define _CHARACTER_H
#include "Glyph.h"

class Visitor;

class Character : public Glyph {
public:
	Character(bool isDirty = false);
	virtual ~Character() = 0;
	Character(const Character& source);
	Character& operator=(const Character& source);

	virtual void Accept(Visitor& visitor);
};

#endif // !_CHARACTER_H