#include "Character.h"
#include "TextOutVisitor.h"

#pragma warning(disable:4996)

Character::Character() {

}

Character::~Character() {

}

Character::Character(const Character& source)
	:Glyph(source) {

}

Character& Character::operator=(const Character& source) {
	Glyph::operator=(source);

	return *this;
}

void Character::Accept(TextOutVisitor& textOutVisitor) {
	textOutVisitor.VisitCharacter(this);
}