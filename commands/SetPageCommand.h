#ifndef _SETPAGECOMMAND_H //guard
#define _SETPAGECOMMAND_H
#include "Command.h"

class CWnd;

class SetPageCommand : public Command {
public:
	SetPageCommand(CWnd* parent);
	virtual ~SetPageCommand();

	virtual void Execute();
};


#endif // !_SETPAGECOMMAND_H
