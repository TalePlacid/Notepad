#include "Glyph.h"

#pragma warning(disable:4996)

Glyph::Glyph() {

}

Glyph::~Glyph() {

}

Glyph::Glyph(const Glyph& source) {

}

Glyph& Glyph::operator=(const Glyph& source) {
	return *this;
}

Long Glyph::Add(Glyph* glyph) {
	return -1;
}

Long Glyph::Add(Long index, Glyph* glyph) {
	return -1;
}

Glyph* Glyph::GetAt(Long index) {
	return 0;
}

Long Glyph::Remove() {
	return 0;
}

Long Glyph::Remove(Long index) {
	return index;
}

Long Glyph::First() {
	return -1;
}

Long Glyph::Previous() {
	return -1;
}

Long Glyph::Next() {
	return -1;
}

Long Glyph::Last() {
	return -1;
}

Long Glyph::Move(Long index) {
	return -1;
}

Glyph* Glyph::operator[](Long index) {
	return 0;
}

Long Glyph::GetCapacity() const {
	return 0;
}

Long Glyph::GetLength() const {
	return 0;
}

Long Glyph::GetCurrent() const {
	return -1;
}

bool Glyph::IsDummyRow() {
	return false;
}