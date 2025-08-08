#ifndef _OPENFINDINGFORMCOMMAND_H //gurad
#define _OPENFINDINGFORMCOMMAND_H
#include "Command.h"

class CWnd;

class OpenFindingFormCommand : public Command {
public:
	OpenFindingFormCommand(CWnd* parent);
	virtual ~OpenFindingFormCommand();

	virtual void Execute();
};

#endif // !_OPENFINDINGFORMCOMMAND_H
