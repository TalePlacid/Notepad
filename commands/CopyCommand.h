#ifndef _COPYCOMMAND_H //guard
#define _COPYCOMMAND_H
#include "Command.h"

class CWnd;

class CopyCommand : public Command {
public:
	CopyCommand(CWnd* parent);
	virtual ~CopyCommand();

	virtual void Execute();
};

#endif // !_COPYCOMMAND_H
