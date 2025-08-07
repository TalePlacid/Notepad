#ifndef _CUTCOMMAND_H //guard
#define _CUTCOMMAND_H
#include "Command.h"

class CWnd;

class CutCommand : public Command {
public:
	CutCommand(CWnd* parent);
	virtual ~CutCommand();

	virtual void Execute();
};

#endif // !_CUTCOMMAND_H
