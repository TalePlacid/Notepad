#ifndef _ERASECOMMAND_H //guard
#define _ERASECOMMAND_H
#include "Command.h"

typedef signed long int Long;

class CWnd;

class EraseCommand : public Command {
public:
	EraseCommand(CWnd* parent);
	virtual ~EraseCommand();
	EraseCommand(const EraseCommand& source);
	EraseCommand& operator=(const EraseCommand& source);

	virtual void Execute();
};


#endif // !_ERASECOMMAND_H
