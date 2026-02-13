#ifndef _CLOSEFINDREPLACECOMMAND_H //guard
#define _CLOSEFINDREPLACECOMMAND_H
#include "Command.h"

class CWnd;
class CFindReplaceDialog;

class CloseFindReplaceCommand : public Command {
public:
	CloseFindReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~CloseFindReplaceCommand();

	virtual void Execute();
private:
	CFindReplaceDialog* findReplaceDialog;
};

#endif // !_CLOSEFINDREPLACECOMMAND_H
