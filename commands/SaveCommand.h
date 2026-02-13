#ifndef _SAVECOMMAND_H //guard
#define _SAVECOMMAND_H
#include "Command.h"
#include <afxwin.h>

class SaveCommand : public Command {
public:
	SaveCommand(CWnd* parent);
	virtual ~SaveCommand();

	virtual void Execute();
};

#endif // !_SAVECOMMAND_H
