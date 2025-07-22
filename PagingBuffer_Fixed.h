#ifndef _PAGINGBUFFERFIXED_H //guard
#define _PAGINGBUFFERFIXED_H
#include "PagingBuffer.h"

typedef signed long int;

class CWnd;

class PagingBuffer_Fixed : public PagingBuffer {
public:
	PagingBuffer_Fixed(CWnd* parent, Long pageSize = 2097152);
	virtual ~PagingBuffer_Fixed();

	virtual void Load();
};

#endif // !_PAGINGBUFFERFIXED_H
