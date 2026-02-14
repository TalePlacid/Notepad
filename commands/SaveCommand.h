#ifndef _SAVECOMMAND_H //guard
#define _SAVECOMMAND_H
#include <afxwin.h>
#include "Command.h"

class SaveCommand : public Command {
public:
	SaveCommand(CWnd* parent);
	virtual ~SaveCommand();

	virtual void Execute();
};

#endif // !_SAVECOMMAND_H
