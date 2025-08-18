#ifndef _CLOSEFINDREPLACECOMMAND_H //guard
#define _CLOSEFINDREPLACECOMMAND_H
#include "FindReplaceCommand.h"

class CWnd;
class CFindReplaceDialog;

class CloseFindReplaceCommand : public FindReplaceCommand {
public:
	CloseFindReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~CloseFindReplaceCommand();

	virtual void Execute();
};

#endif // !_CLOSEFINDREPLACECOMMAND_H
