#ifndef _LOADNEXTCOMMAND_H //guard
#define _LOADNEXTCOMMAND_H
#include "Command.h"

class CWnd;

class LoadNextCommand : public Command {
public:
	LoadNextCommand(CWnd* parent);
	virtual ~LoadNextCommand();

	virtual void Execute();
};


#endif // !_LOADNEXTCOMMAND_H
