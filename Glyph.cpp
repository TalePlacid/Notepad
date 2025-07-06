#include "Glyph.h"

#pragma warning(disable:4996)

Glyph::Glyph() {
	this->isSelected = false;
}

Glyph::~Glyph() {

}

Glyph::Glyph(const Glyph& source) {
	this->isSelected = source.isSelected;
}

Glyph& Glyph::operator=(const Glyph& source) {
	this->isSelected = source.isSelected;

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

bool Glyph::IsDummyRow() {
	return false;
}

bool Glyph::IsWordCharacter() {
	return false;
}

bool Glyph::IsMultiByteCharacter() {
	return false;
}

void Glyph::Accept(Visitor& visitor) {

}

bool Glyph::Select(bool isSelecting) {
	this->isSelected = isSelecting;

	return this->isSelected;
}

Glyph* Glyph::operator[](Long index) {
	return 0;
}

Glyph::operator char*() const{
	return 0;
}