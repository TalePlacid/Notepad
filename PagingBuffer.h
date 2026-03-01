#ifndef _PAGINGBUFFER_H //guard
#define _PAGINGBUFFER_H
#define PAGE_MULTIPLE 4
#define PAGE_ROWCOUNT 54
#include <afxwin.h>
#include <cstdio>

typedef signed long int Long;

class Glyph;

class PagingBuffer {
public:
	PagingBuffer(CWnd* parent, TCHAR(*sourceContents), Long sourceCount, Long pageSize = 2097152);
	~PagingBuffer();

	Glyph* LoadPrevious();
	Glyph* LoadNext();
	Long Add(char(*character));
	Long Add(CString str);
	Long Remove();
	Long Remove(Long toOffset);
	void Clear();
	Long Replace(Long offset, CString str);
	Long CountRow(Long offset);

	Long First();
	Long Previous(Long count = 1);
	Long Next(Long count = 1);
	Long Last();
	Long Move(Long index);

	Long FirstRow();
	Long PreviousRow(Long count = 1);
	Long NextRow(Long count = 1);
	Long LastRow();

	Long MarkSelectionBegin();
	Long UnmarkSelectionBegin();
	bool BeginSelectionIfNeeded();
	bool EndSelectionIfCollapsed();

	Long MoveOffset(Long offset);

	Long CacheRowStartIndex(Long difference);

	CString MakeSelectedString();
	CString GetFullText();

	CWnd* GetParent() const;
	FILE* GetFile() const;
	Long GetFileEndOffset() const;
	Long GetRowStartIndex() const;
	Long GetCurrentOffset() const;
	Long GetSelectionBeginOffset() const;
protected:
	CWnd* parent;
	CString tempPath;
	Long pageSize;
	FILE* file;
	Long rowStartIndex;
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

inline Long PagingBuffer::GetSelectionBeginOffset() const {
	return this->selectionBeginOffset;
}

#endif // !_PAGINGBUFFER_H

