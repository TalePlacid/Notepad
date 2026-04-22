#include <afxwin.h>
#include "NoteWidthCache.h"
#include "RowWidthCache.h"
#include "../NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

NoteWidthCache::NoteWidthCache(CWnd* parent, Long capacity)
	:rowWidthCaches(capacity) {
	this->parent = parent;
	this->capacity = capacity;
	this->length = 0;
}

NoteWidthCache::NoteWidthCache(CWnd* parent, Glyph* note)
	:rowWidthCaches(note->GetCapacity()) {
	this->parent = parent;
	Long i = 0;
	while (i < note->GetLength())
	{
		this->rowWidthCaches.Store(i, new RowWidthCache(parent, note->GetAt(i)));
		i++;
	}

	this->capacity = note->GetCapacity();
	this->length = note->GetLength();
}

NoteWidthCache::~NoteWidthCache() {
	Long i = 0;
	while (i < this->length)
	{
		if (this->rowWidthCaches[i] != 0)
		{
			delete this->rowWidthCaches[i];
			this->rowWidthCaches[i] = 0;
		}
		i++;
	}
}

Long NoteWidthCache::Add() {
	RowWidthCache* rowWidthCache = new RowWidthCache(this->parent);

	Long index = -1;
	if (this->length < this->capacity)
	{
		index = this->rowWidthCaches.Store(this->length, rowWidthCache);
	}
	else
	{
		index = this->rowWidthCaches.AppendFromRear(rowWidthCache);
		this->capacity++;
	}
	this->length++;

	return index;
}

Long NoteWidthCache::Add(Long index, Glyph* row) {
	RowWidthCache* rowWidthCache;
	if (row == 0)
	{
		rowWidthCache = new RowWidthCache(this->parent);
	}
	else
	{
		rowWidthCache = new RowWidthCache(this->parent, row);
	}

	Long ret = this->rowWidthCaches.Insert(index, rowWidthCache);
	this->capacity++;
	this->length++;

	return ret;
}

Long NoteWidthCache::Remove() {
	if (this->rowWidthCaches[this->length - 1] != 0)
	{
		delete this->rowWidthCaches[this->length - 1];
	}
	Long index = this->rowWidthCaches.Delete(this->length - 1);
	this->length--;
	this->capacity--;

	return index;
}

Long NoteWidthCache::Remove(Long index) {
	if (this->rowWidthCaches[index] != 0)
	{
		delete this->rowWidthCaches[index];
	}
	Long ret = this->rowWidthCaches.Delete(index);
	this->length--;
	this->capacity--;

	return ret;
}

RowWidthCache* NoteWidthCache::GetAt(Long index) {
	return this->rowWidthCaches.GetAt(index);
}

Long NoteWidthCache::MarkDirty(Long index) {
	this->rowWidthCaches[index]->MarkDirty();

	return index;
}

void NoteWidthCache::MarkAllDirty() {
	Long i = 0;
	while (i < this->length)
	{
		this->rowWidthCaches[i]->MarkDirty();
		i++;
	}
}

Long NoteWidthCache::CleanDirty(Long index) {
	this->rowWidthCaches[index]->CleanDirty();

	return index;
}

Long NoteWidthCache::AppendFromFront(const NoteWidthCache& other) {
	NoteWidthCache& other_ = const_cast<NoteWidthCache&>(other);
	Long count = 0;
	Long i = other.length - 1;
	if (i == 0 && other_.rowWidthCaches[i]->IsEmptyRow())
	{
		i--;
	}

	while (i >= 0)
	{
		RowWidthCache* rowWidthCache = new RowWidthCache(*other_.rowWidthCaches[i]);
		this->rowWidthCaches.AppendFromFront(rowWidthCache);
		this->capacity++;
		this->length++;
		count++;
		i--;
	}

	return count;
}

Long NoteWidthCache::AppendFromRear(const NoteWidthCache& other) {
	NoteWidthCache& other_ = const_cast<NoteWidthCache&>(other);
	Long count = 0;
	Long i = 0;
	if (other.length == 1 && other_.rowWidthCaches[i]->IsEmptyRow())
	{
		i++;
	}

	while (i < other.length)
	{
		RowWidthCache* rowWidthCache = new RowWidthCache(*other_.rowWidthCaches[i]);
	
		if (this->length < this->capacity)
		{
			this->rowWidthCaches.Store(this->length, rowWidthCache);
		}
		else
		{
			this->rowWidthCaches.AppendFromRear(rowWidthCache);
			this->capacity++;
		}
		this->length++;
		count++;

		i++;
	}

	return count;
}

Long NoteWidthCache::TruncateAfter(Long index) {
	Long count = 0;
	while (this->length > index + 1)
	{
		if (this->rowWidthCaches[index + 1] != 0)
		{
			delete this->rowWidthCaches[index + 1];
		}

		this->rowWidthCaches.Delete(index + 1);
		this->length--;
		this->capacity--;
		count++;
	}

	return count;
}

Long NoteWidthCache::TruncateBefore(Long index) {
	Long count = 0;
	while (count < index)
	{
		if (this->rowWidthCaches[0] != 0)
		{
			delete this->rowWidthCaches[0];
		}

		this->rowWidthCaches.Delete(0);
		this->length--;
		this->capacity--;
		count++;
	}

	return count;
}

RowWidthCache* NoteWidthCache::operator[](Long index) {
	return this->rowWidthCaches[index];
}
