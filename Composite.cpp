#include "Composite.h"

#pragma warning(disable:4996)

Composite::Composite(Long capacity)
	:glyphs(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->current = -1;
}

Composite::~Composite() {
	Long i = 0;
	while (i < this->length)
	{
		delete this->glyphs[i];
		i++;
	}
}

Composite::Composite(const Composite& source)
	:Glyph(source), glyphs(source.capacity) {
	Glyph* glyph;
	Long i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source)[i]->Clone();
		this->glyphs.Store(i, glyph);
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

Composite& Composite::operator=(const Composite& source) {
	Glyph::operator=(source);

	Long i = 0;
	while (i < this->length)
	{
		delete this->glyphs[i];
		i++;
	}

	this->glyphs = source.glyphs;

	Glyph* glyph;
	i = 0;
	while (i < source.length)
	{
		glyph = const_cast<Composite&>(source)[i]->Clone();
		this->glyphs.Store(i, glyph);
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}

Long Composite::Add(Glyph* glyph) {
	Long index;
	if (this->length < this->capacity)
	{
		index = this->glyphs.Store(this->length, glyph);
	}
	else
	{
		index = this->glyphs.AppendFromRear(glyph);
		(this->capacity)++;
	}
	(this->length)++;
	(this->current)++;

	return index;
}

Long Composite::Add(Long index, Glyph* glyph) {
	if (index <= 0)
	{
		index = this->glyphs.AppendFromFront(glyph);
	}
	else
	{
		index = this->glyphs.Insert(index, glyph);
	}
	(this->capacity)++;
	(this->length)++;
	(this->current)++;

	return index;
}

Glyph* Composite::GetAt(Long index) {
	return this->glyphs.GetAt(index);
}

Long Composite::Remove() {
	if (this->glyphs[this->length - 1] != 0)
	{
		delete this->glyphs[this->length - 1];
	}

	Long index = this->glyphs.Delete(this->length - 1);
	(this->length)--;
	(this->capacity)--;
	(this->current)--;

	return index;
}

Long Composite::Remove(Long index) {
	if (this->glyphs[index] != 0)
	{
		delete this->glyphs[index];
	}

	index = this->glyphs.Delete(index);
	(this->length)--;
	(this->capacity)--;
	(this->current)--;

	return index;
}

Long Composite::First() {
	this->current = -1;

	return this->current;
}

Long Composite::Previous() {
	(this->current)--;
	if (this->current < -1)
	{
		this->current = -1;
	}

	return this->current;
}

Long Composite::Next() {
	(this->current)++;
	if (this->current >= this->length)
	{
		this->current = this->length - 1;
	}

	return this->current;
}

Long Composite::Last() {
	this->current = this->length - 1;

	return this->current;
}

Long Composite::Move(Long index) {
	this->current = index;

	return this->current;
}

void Composite::Accept(Visitor& visitor) {
	Long i = 0;
	while (i < this->length)
	{
		this->glyphs[i]->Accept(visitor);
		i++;
	}
}

bool Composite::Select(bool isSelecting) {
	Long i = 0;
	while (i < this->length)
	{
		this->glyphs[i]->Select(isSelecting);
		i++;
	}
	this->isSelected = isSelecting;

	return this->isSelected;
}

Glyph* Composite::operator[](Long index) {
	return this->glyphs[index];
}