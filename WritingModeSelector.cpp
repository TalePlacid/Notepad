#include "WritingModeSelector.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

WritingModeSelector::WritingModeSelector() {

}

WritingModeSelector::~WritingModeSelector() {

}

AppID WritingModeSelector::DetermineWritingMode(const PagingBuffer* pagingBuffer) {
	AppID nID = AppID::ID_COMMAND_INSERT_AT_CARET;
	if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetFileEndOffset())
	{
		nID = AppID::ID_COMMAND_WRITE_AT_END;
	}

	return nID;
}
