#ifndef _ERASERANGECOMMAND_H //guard
#define _ERASERANGECOMMAND_H
#include "Command.h"

class CWnd;

class EraseRangeCommand : public Command {
public:
	EraseRangeCommand(CWnd* parent);
	virtual ~EraseRangeCommand();

	virtual void Execute();
};

#endif // !_ERASERANGECOMMAND_H
