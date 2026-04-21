#ifndef _ROWWIDTHCACHE_H //guard
#define _ROWWIDTHCACHE_H

typedef signed long int Long;

class CWnd;
class Glyph;

class RowWidthCache {
public:
	RowWidthCache(CWnd* parent, Long capacity = 256);
	RowWidthCache(CWnd* parent, Glyph *row);
	~RowWidthCache();
	RowWidthCache(const RowWidthCache& source);
	RowWidthCache& operator=(const RowWidthCache& source);

	void Recalculate(Glyph* row);
	Long GetAt(Long index);
	
	bool MarkDirty();
	bool CleanDirty();

	bool IsDirty();
	bool IsEmptyRow() const;

	Long GetCapacity() const;
	Long GetLength() const;

	Long operator[](Long index);
private:
	CWnd* parent;
	Long(*prefixWidths);
	Long capacity;
	Long length;
	bool isDirty;
};

inline bool RowWidthCache::IsDirty() {
	return this->isDirty;
}

inline bool RowWidthCache::IsEmptyRow() const {
	return this->length == 1;
}

inline Long RowWidthCache::GetCapacity() const {
	return this->capacity;
}

inline Long RowWidthCache::GetLength() const {
	return this->length;
}

#endif // !_ROWWIDTHCACHE_H
