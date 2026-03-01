#ifndef _HISTORYBOOK_H //guard
#define _HISTORYBOOK_H
#include "DropOldestStack.h"
#include "DateTime.h"

typedef signed long int Long;

class Command;
class CWnd;

class HistoryBook {
public:
	HistoryBook(Long capacity = 128);
	~HistoryBook();
	HistoryBook(const HistoryBook& source);
	HistoryBook& operator=(const HistoryBook& source);

	Command* Bind(Command* command);
	Command** Push(Command* history);
	Command* Pop();
	bool IsEmpty();
	Command** Clear();
	Command** Peek();

	DateTime& GetLatestPushTime() const;
private:
	DropOldestStack<Command*> histories;
	Long capacity;
	Long length;
	DateTime latestPushTime;
};

inline DateTime& HistoryBook::GetLatestPushTime() const {
	return const_cast<DateTime&>(this->latestPushTime);
}

#endif // !_HISTORYBOOK_H

