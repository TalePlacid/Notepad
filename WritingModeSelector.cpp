#include "WritingModeSelector.h"
#include "resource.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

WritingModeSelector::WritingModeSelector() {

}

WritingModeSelector::~WritingModeSelector() {

}

UINT WritingModeSelector::DetermineWritingMode(const PagingBuffer* pagingBuffer) {
	UINT nID = ID_COMMAND_INSERTATCARET;
	if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetFileEndOffset())
	{
		nID = ID_COMMAND_WRITEATEND;
	}

	return nID;
}