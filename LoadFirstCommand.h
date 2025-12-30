#ifndef _LOADFIRSTCOMMAND_H //guard
#define _LOADFIRSTCOMMAND_H
#include "Command.h"

class CWnd;

class LoadFirstCommand : public Command {
public:
	LoadFirstCommand(CWnd* parent);
	virtual ~LoadFirstCommand();

	virtual void Execute();
	virtual bool NeedScrollBarUpdate();
};

inline bool LoadFirstCommand::NeedScrollBarUpdate() {
	return false;
}

#endif // !_LOADFIRSTCOMMAND_H
