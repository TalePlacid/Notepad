#ifndef _WRITINGMODESELECTOR_H //guard
#define _WRITINGMODESELECTOR_H
#include <afxwin.h>
#include "AppID.h"

class PagingBuffer;

class WritingModeSelector {
public:
	WritingModeSelector();
	~WritingModeSelector();

	static AppID DetermineWritingMode(const PagingBuffer* pagingBuffer);
};

#endif // !_WRITINGMODESELECTOR_H

