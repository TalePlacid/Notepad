#ifndef _HISTORY_H //guard
#define _HISTORY_H
#include "MacroCommand.h"

typedef signed long int Long;

class History : public MacroCommand {
public:
	History(CWnd* parent = 0, Long capacity = 64);
	virtual ~History();
	History(const History& source);
	History& operator=(const History& source);
};

#endif // !_HISTORY_H
