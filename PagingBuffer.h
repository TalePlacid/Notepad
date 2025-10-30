#ifndef _PAGINGBUFFER_H //guard
#define _PAGINGBUFFER_H
#define PAGE_MULTIPLE 4
#define PAGE_ROWCOUNT 54
#include <afxwin.h>
#include <cstdio>
#include "Position.h" 

typedef signed long int Long;

class Glyph;

class PagingBuffer {
public:
	PagingBuffer(CWnd* parent, Long pageSize = 2097152);
	~PagingBuffer();

	Glyph* Load();
	Glyph* LoadNext();
	Long Add(char(*character));
	Long Add(CString str);
	Long Remove();
	Long Remove(Long toOffset);
	Long Replace(Long offset, CString str);
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
	Long NextRow(Long count = 1);
	Position& LastRow();
	Position& MoveRow(Long index);

	Long MarkSelectionBegin();
	Long UnmarkSelectionBegin();

	bool IsOnPage(Long offset);
	Long MoveOffset(Long offset);

	Long CacheRowStartIndex(Long difference);

	CString MakeSelectedString();
	CString GetFullText();

	CWnd* GetParent() const;
	FILE* GetFile() const;
	Long GetFileEndOffset() const;
	Long GetRowStartIndex() const;
	Long GetStartOffset() const;
	Long GetCurrentOffset() const;
	Long GetEndOffset() const;
	Position& GetStart() const;
	Position& GetCurrent() const;
	Position& GetEnd() const;
	Long GetSelectionBeginOffset() const;
protected:
	CWnd* parent;
	Long pageSize;
	FILE* file;
	Long rowStartIndex;
	Long startOffset;
	Long endOffset;
	Position current;
	Position start;
	Position end;
	Long selectionBeginOffset;
};

inline CWnd* PagingBuffer::GetParent() const {
	return const_cast<CWnd*>(this->parent);
}

inline FILE* PagingBuffer::GetFile() const{
	return const_cast<FILE*>(this->file);
}

inline Long PagingBuffer::GetRowStartIndex() const {
	return this->rowStartIndex;
}

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
