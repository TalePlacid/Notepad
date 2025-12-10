#include "Composite.h"

#pragma warning(disable:4996)

Composite::Composite(Long capacity)
	:glyphs(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->current = -1;
	this->marked = -1;
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
	this->marked = source.marked;
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
	this->marked = source.marked;

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

Long Composite::TruncateAfter(Long index) {
	Long count = 0;
	while (this->length > index + 1)
	{
		if (this->glyphs[index + 1] != NULL)
		{
			delete this->glyphs[index + 1];
		}
		this->glyphs.Delete(index + 1);
		(this->length)--;
		(this->capacity)--;
		count++;
	}

	if (this->current > index)
	{
		this->current = index;
	}

	return count;
}

Long Composite::TruncateBefore(Long index) {
	Long count = 0;
	while (count < index)
	{
		if (this->glyphs[0] != NULL)
		{
			delete this->glyphs[0];
		}
		this->glyphs.DeleteFromFront();
		(this->length)--;
		(this->capacity)--;
		count++;
	}

	if (this->current < index)
	{
		this->capacity = 0;
	}
	else
	{
		this->current -= index;
	}

	return count;
}

Long Composite::Replace(Long index, Glyph* glyph) {
	if (this->glyphs[index] != NULL)
	{
		delete this->glyphs[index];
	}
	this->glyphs[index] = glyph;

	return index;
}

Long Composite::First() {
	this->current = 0;

	return this->current;
}

Long Composite::Previous() {
	(this->current)--;
	if (this->current < 0)
	{
		this->current = 0;
	}

	return this->current;
}

Long Composite::Next() {
	(this->current)++;
	if (this->current > this->length)
	{
		this->current = this->length;
	}

	return this->current;
}

Long Composite::Last() {
	this->current = this->length;

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

	return isSelecting;
}

Long Composite::SelectRange(Long start, Long end, bool isSelecting) {
	Long count = 0;
	Long i = start;
	while (i <= end && i < this->length)
	{
		this->glyphs[i]->Select(isSelecting);
		count++;
		i++;
	}

	return count;
}

Long Composite::Mark(Long index) {
	this->marked = index;

	return this->marked;
}

Long Composite::Unmark() {
	this->marked = -1;

	return this->marked;
}

Glyph* Composite::operator[](Long index) {
	return this->glyphs[index];
}