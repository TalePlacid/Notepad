#ifndef _HISTORYBINDER_H //guard
#define _HISTORYBINDER_H
#include "Date.h"
#include "History.h"

class CWnd;
class Command;

class HistoryBinder {
public:
	HistoryBinder();
	~HistoryBinder();

	Long Bind(Command* command);
	History Commit();
	bool IsBindable(Command* command);

	Long GetLength() const;
private:
	Date bindingStartTime;
	History history;
};

inline Long HistoryBinder::GetLength() const {
	return this->history.GetLength();
}

#endif // !_HISTORYBINDER_H
