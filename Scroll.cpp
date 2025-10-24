#include "Scroll.h"

#pragma warning(disable:4996)

Scroll::Scroll() {
	this->min = 0;
	this->max = 0;
	this->page = 0;
	this->pos = 0;
}

Scroll::Scroll(Long max, Long page, Long pos, Long min) {
	this->min = min;
	this->max = max;
	this->page = page;
	this->pos = pos;
}

Scroll::~Scroll() {

}

Scroll::Scroll(const Scroll& source) {
	this->min = source.min;
	this->max = source.max;
	this->page = source.page;
	this->pos = source.pos;
}

Scroll& Scroll::operator=(const Scroll& source) {
	this->min = source.min;
	this->max = source.max;
	this->page = source.page;
	this->pos = source.pos;

	return *this;
}

void Scroll::ResizeRange(Long max, Long min) {
	this->min = min;
	this->max = max;
}

Long Scroll::ResizePage(Long page) {
	this->page = page;

	return this->page;
}

Long Scroll::Move(Long pos) {
	this->pos = pos;

	return this->pos;
}

Long Scroll::LineUp(Long distance) {
	this->pos -= distance;
	if (this->pos < this->min)
	{
		this->pos = this->min;
	}

	return this->pos;
}

Long Scroll::LineDown(Long distance) {
	this->pos += distance;
	if (this->pos > this->max)
	{
		this->pos = this->max;
	}

	return this->pos;
}

Long Scroll::PageUp() {
	this->pos -= this->page;
	if (this->pos < this->min)
	{
		this->pos = this->min;
	}

	return this->pos;
}

Long Scroll::PageDown() {
	this->pos += this->page;
	if (this->pos > this->max)
	{
		this->pos = this->max;
	}

	return this->pos;
}