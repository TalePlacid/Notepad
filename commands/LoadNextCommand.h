#ifndef _LOADNEXTCOMMAND_H //guard
#define _LOADNEXTCOMMAND_H
#include "Command.h"

class CWnd;

class LoadNextCommand : public Command {
public:
	LoadNextCommand(CWnd* parent);
	virtual ~LoadNextCommand();

	virtual void Execute();
	virtual bool NeedScrollBarUpdate();
};

inline bool LoadNextCommand::NeedScrollBarUpdate() {
	return false;
}

#endif // !_LOADNEXTCOMMAND_H
