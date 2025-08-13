#ifndef _OPENFINDDIALOGCOMMAND_H //gurad
#define _OPENFINDDIALOGCOMMAND_H
#include "Command.h"

class CWnd;

class OpenFindDialogCommand : public Command {
public:
	OpenFindDialogCommand(CWnd* parent);
	virtual ~OpenFindDialogCommand();

	virtual void Execute();
};

#endif // !_OPENFINDDIALOGCOMMAND_H
