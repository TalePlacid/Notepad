#ifndef _PAGINGBUFFER_H //guard
#define _PAGINGBUFFER_H
#include <cstdio>
#include "Position.h"
#define PAGE_MULTIPLE 4

typedef signed long int Long;

class CWnd;

class PagingBuffer {
public:
	PagingBuffer(CWnd* parent, Long pageSize = 2097152);
	~PagingBuffer();

	void Load();
	void Save();
	Long CountRow(Long offset);
	bool IsAboveBottomLine();
	bool IsBelowTopLine();

	Position& First();
	Position& Previous();
	Position& Next();
	Position& Last();
	Position& Move(Long index);

	Position& FirstRow();
	Position& PreviousRow();
	Position& NextRow();
	Position& LastRow();
	Position& MoveRow(Long index);

	Long MarkSelectionBegin();
	Long UnmarkSelectionBegin();

	Long GetFileEnd() const;
	Long GetStartOffset() const;
	Long GetCurrentOffset() const;
	Long GetEndOffset() const;
	Position& GetStart() const;
	Position& GetCurrent() const;
	Position& GetEnd() const;
	Long GetSelectionBeginOffset() const;
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
	Long selectionBeginOffset;
};

inline Long PagingBuffer::GetStartOffset() const {
	return this->startOffset;
}

inline Long PagingBuffer::GetEndOffset() const {
	return this->endOffset;
}

inline Position& PagingBuffer::GetStart() const {
	return const_cast<Position&>(this->start);
}

inline Position& PagingBuffer::GetCurrent() const {
	return const_cast<Position&>(this->current);
}

inline Position& PagingBuffer::GetEnd() const {
	return const_cast<Position&>(this->end);
}

inline Long PagingBuffer::GetSelectionBeginOffset() const {
	return this->selectionBeginOffset;
}

#endif // !_PAGINGBUFFER_H
