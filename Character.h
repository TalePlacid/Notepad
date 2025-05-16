#ifndef _CHARACTER_H //guard
#define _CHARACTER_H
#include "Glyph.h"

class Character : public Glyph {
public:
	Character();
	virtual ~Character() = 0;
	Character(const Character& source);
	Character& operator=(const Character& source);
};

#endif // !_CHARACTER_H