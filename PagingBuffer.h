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

#endif // !_PAGINGBUFFER_H
