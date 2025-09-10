#ifndef _HISTORYBOOK_H //guard
#define _HISTORYBOOK_H
#include "DropOldestStack.h"

typedef signed long int Long;

class Command;
class CWnd;

class HistoryBook {
public:
	HistoryBook(Long capacity = 128);
	~HistoryBook();
	HistoryBook(const HistoryBook& source);
	HistoryBook& operator=(const HistoryBook& source);

	Command** Push(Command* history);
	Command* Pop();
	bool IsEmpty();
	Command** Clear();
private:
	DropOldestStack<Command*> histories;
	Long capacity;
	Long length;
};

#endif // !_HISTORYBOOK_H
