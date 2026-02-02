#ifndef _OPENCOMMAND_H //guard
#define _OPENCOMMAND_H
#include "Command.h"

class OpenCommand : public Command {
public:
	OpenCommand(CWnd* parent);
	virtual ~OpenCommand();

	virtual void Execute();
};

#endif // !_OPENCOMMAND_H
