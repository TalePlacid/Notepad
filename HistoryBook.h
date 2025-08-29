#ifndef _HISTORYBOOK_H //guard
#define _HISTORYBOOK_H
#include "DropOldestStack.h"
#include "History.h"

typedef signed long int Long;

class CWnd;

class HistoryBook {
public:
	HistoryBook(Long capacity = 128);
	~HistoryBook();
	HistoryBook(const HistoryBook& source);
	HistoryBook& operator=(const HistoryBook& source);

	History* Push(History history);
	History Pop();
	bool IsEmpty();
private:
	DropOldestStack<History> histories;
	DropOldestStack<History> undoneHistories;
	History* top;
	Long capacity;
	Long historyLength;
	Long undoneHistoryLength;
};

#endif // !_HISTORYBOOK_H
