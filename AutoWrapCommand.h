#ifndef _AUTOWRAPCOMMAND_H //gurad
#define _AUTOWRAPCOMMAND_H
#include "Command.h"

class CWnd;

class AutoWrapCommand : public Command{
public:
	AutoWrapCommand(CWnd* parent);
	virtual ~AutoWrapCommand();

	virtual void Execute();
};


#endif // !_AUTOWRAPCOMMAND_H
