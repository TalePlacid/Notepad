#ifndef _MULTIBYTEWIDTHCACHE_H //guard
#define _MULTIBYTEWIDTHCACHE_H
#include "Array.h"
#include "MultiByteWidth.h"

class CWnd;

class MultiByteWidthCache {
public:
	MultiByteWidthCache(CWnd* parent, Long capacity = 512);
	~MultiByteWidthCache();

	Long GetWidth(char(*character));

	Long GetCapacity() const;
	Long GetLength() const;
private:
	CWnd* parent;
	Array<MultiByteWidth> multiByteWidths;
	Long capacity;
	Long length;

	static int CompareMultiByteWidths(void* one, void* other);
};

inline Long MultiByteWidthCache::GetCapacity() const {
	return this->capacity;
}

inline Long MultiByteWidthCache::GetLength() const {
	return this->length;
}

#endif // !_MULTIBYTEWIDTHCACHE_H
