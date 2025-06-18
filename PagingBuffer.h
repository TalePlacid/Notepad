#ifndef _PAGINGBUFFER_H //guard
#define _PAGINGBUFFER_H
#include <cstdio>
#include "Position.h"

typedef signed long int Long;

class CWnd;

class PagingBuffer {
public:
	PagingBuffer(CWnd* parent, Long pageSize = 65536);
	~PagingBuffer();

	void Load();
	void Save();

	Position& First();
	Position& Previous();
	Position& Next();
	Position& Last();
	Position& Move(Position index);

	Position& GetStart() const;
	Position& GetEnd() const;

	bool IsOnView();
private:
	CWnd* parent;
	Long pageSize;
	FILE* file;
	Long startOffset;
	Long endOffset;
	Position current;
	Position start;
	Position end;
	bool isDirty;
};

inline Position& PagingBuffer::GetStart() const {
	return const_cast<Position&>(this->start);
}

inline Position& PagingBuffer::GetEnd() const {
	return const_cast<Position&>(this->end);
}

#endif // !_PAGINGBUFFER_H
