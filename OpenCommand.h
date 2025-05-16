#ifndef _OPENCOMMAND_H //guard
#define _OPENCOMMAND_H
#include "Command.h"

class OpenCommand : public Command {
public:
	OpenCommand(CWnd* parent);
	virtual ~OpenCommand();
	OpenCommand(const OpenCommand& source);
	OpenCommand& operator=(const OpenCommand& source);

	virtual void Execute();
	virtual void Unexecute();
};

#endif // !_OPENCOMMAND_H
