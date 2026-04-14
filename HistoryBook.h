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
	void StopBinding();
	Command** Push(Command* history);
	Command* Pop();
	bool IsEmpty();
	Command** Clear();
	Command** Peek();

	DateTime& GetBindingStartTime() const;
private:
	DropOldestStack<Command*> histories;
	Long capacity;
	Long length;
	bool isBindingStopped;
	DateTime bindingStartTime;
};

inline DateTime& HistoryBook::GetBindingStartTime() const {
	return const_cast<DateTime&>(this->bindingStartTime);
}

#endif // !_HISTORYBOOK_H

