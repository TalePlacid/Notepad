#ifndef _NOTEWIDTHCACHE_H //guard
#define _NOTEWIDTHCACHE_H
#include "../Observer.h"
#include "../Array.h"

class RowWidthCache;
class Glyph;
class CWnd;

class NoteWidthCache : public Observer{
public:
	NoteWidthCache(CWnd* parent, Long capacity = 256);
	NoteWidthCache(CWnd* parent, Glyph* note);
	~NoteWidthCache();

	Long Add();
	Long Add(Long index, Glyph* row = 0);
	Long Remove();
	Long Remove(Long index);
	RowWidthCache* GetAt(Long index);
	Long MarkDirty(Long index);
	Long CleanDirty(Long index);

	virtual void Update(Subject* subject, string interest);

	Long AppendFromFront(const NoteWidthCache& other);
	Long AppendFromRear(const NoteWidthCache& other);
	Long TruncateAfter(Long index);
	Long TruncateBefore(Long index);

	RowWidthCache* operator[](Long index);

	Long GetCapacity() const;
	Long GetLength() const;
private:
	CWnd* parent;
	Array<RowWidthCache*> rowWidthCaches;
	Long capacity;
	Long length;
};

inline Long NoteWidthCache::GetCapacity() const {
	return this->capacity;
}

inline Long NoteWidthCache::GetLength() const {
	return this->length;
}

#endif // !_NOTEWIDTHCACHE_H
