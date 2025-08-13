#ifndef _FINDCOMMAND_H //guard
#define _FINDCOMMAND_H
#include "FindReplaceCommand.h"

class CWnd;
class CFindReplaceDialog;

class FindCommand : public FindReplaceCommand {
public:
	FindCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~FindCommand();

	virtual void Execute();
};

#endif // !_FINDCOMMAND_H
