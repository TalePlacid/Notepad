#ifndef _SAVECOMMAND_H //guard
#define _SAVECOMMAND_H
#include "Command.h"
#include <afxwin.h>

class SaveCommand : public Command {
public:
	SaveCommand(CWnd* parent);
	virtual ~SaveCommand();
	SaveCommand(const SaveCommand& source);
	SaveCommand& operator=(const SaveCommand& source);

	virtual void Execute();
	virtual void Unexecute();
};

#endif // !_SAVECOMMAND_H
