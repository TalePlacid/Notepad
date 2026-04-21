#include <afxwin.h>
#include "RowWidthCache.h"
#include "Glyph.h"
#include "../NotepadForm.h"
#include "../SizeCalculator.h"

#pragma warning(disable:4996)

RowWidthCache::RowWidthCache(CWnd* parent, Long capacity) {
	this->parent = parent;
	this->prefixWidths = new Long[capacity];
	this->prefixWidths[0] = 0;
	this->capacity = capacity;
	this->length = 1;
	this->isDirty = false;
}

RowWidthCache::RowWidthCache(CWnd* parent, Glyph* row) {
	this->parent = parent;
	this->prefixWidths = new Long[row->GetCapacity() + 1];
	this->capacity = row->GetCapacity() + 1;
	this->isDirty = false;

	//첫번째
	Long previousWidth;
	Long currentWidth = 0;
	this->prefixWidths[0] = currentWidth;
	this->length = 1;

	//나머지
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long i = 1;
	while (i <= row->GetLength())
	{
		previousWidth = currentWidth;
		currentWidth += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i - 1)), previousWidth);
		this->prefixWidths[i] = currentWidth;
		this->length++;
		i++;
	}
}

RowWidthCache::~RowWidthCache() {
	if (this->prefixWidths != 0)
	{
		delete[] this->prefixWidths;
	}
}

RowWidthCache::RowWidthCache(const RowWidthCache& source) {
	this->parent = source.parent;
	this->prefixWidths = new Long[source.capacity];

	Long i = 0;
	while (i < source.length)
	{
		this->prefixWidths[i] = source.prefixWidths[i];
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->isDirty = source.isDirty;
}

RowWidthCache& RowWidthCache::operator=(const RowWidthCache& source) {
	this->parent = source.parent;
	if (this->prefixWidths != 0)
	{
		delete[] this->prefixWidths;
		this->prefixWidths = 0;
	}
	this->prefixWidths = new Long[source.capacity];

	Long i = 0;
	while (i < source.length)
	{
		this->prefixWidths[i] = source.prefixWidths[i];
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->isDirty = source.isDirty;

	return *this;
}

void RowWidthCache::Recalculate(Glyph* row) {
	//첫번째
	Long previousWidth;
	Long currentWidth = 0;
	this->prefixWidths[0] = currentWidth;
	this->length = 1;

	//나머지
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long i = 1;
	while (i <= row->GetLength())
	{
		previousWidth = currentWidth;
		currentWidth += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i - 1)), previousWidth);
		if (this->length >= this->capacity)
		{
			Long(*doubled) = new Long[this->capacity * 2];
			Long j = 0;
			while (j < this->length)
			{
				doubled[j] = this->prefixWidths[j];
				j++;
			}

			if (this->prefixWidths != 0)
			{
				delete[] this->prefixWidths;
			}

			this->prefixWidths = doubled;
			this->capacity *= 2;
		}
		this->prefixWidths[i] = currentWidth;
		this->length++;
		i++;
	}
}

Long RowWidthCache::GetAt(Long index) {
	return this->prefixWidths[index];
}

bool RowWidthCache::MarkDirty() {
	this->isDirty = true;

	return this->isDirty;
}

bool RowWidthCache::CleanDirty() {
	this->isDirty = false;

	return this->isDirty;
}

Long RowWidthCache::operator[](Long index) {
	return this->prefixWidths[index];
}