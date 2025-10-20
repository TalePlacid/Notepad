#ifndef _UPDATESCROLLBARSCOMMAND_H //guard
#define _UPDATESCROLLBARSCOMMAND_H
#include "Command.h"
#include "ScrollController.h"

class CWnd;

class UpdateScrollBarsCommand : public Command {
public:
	UpdateScrollBarsCommand(CWnd* parent, ScrollBarNeeded scrollBarNeeded);
	virtual ~UpdateScrollBarsCommand();

	virtual void Execute();
private:
	ScrollBarNeeded scrollBarNeeded;
};

#endif // !_UPDATESCROLLBARSCOMMAND_H
