#ifndef _NEWWINDOWCOMMAND_H
#define _NEWWINDOWCOMMAND_H
#include "Command.h"

class CWnd;

class NewWindowCommand : public Command {
public:
	NewWindowCommand(CWnd* parent);
	virtual ~NewWindowCommand();

	virtual void Execute();
};

#endif // !_NEWWINDOWCOMMAND_H
