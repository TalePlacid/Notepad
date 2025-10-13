#ifndef _CREATESCROLLBARSCOMMAND_H //guard
#define _CREATESCROLLBARSCOMMAND_H
#include "Command.h"

class CWnd;

class CreateScrollBarsCommand : public Command {
public:
	CreateScrollBarsCommand(CWnd* parent);
	virtual ~CreateScrollBarsCommand();

	virtual void Execute();
};

#endif // !_CREATESCROLLBARSCOMMAND_H
