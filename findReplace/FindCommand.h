#ifndef _FINDCOMMAND_H //guard
#define _FINDCOMMAND_H
#include "Command.h"

class CWnd;
class CFindReplaceDialog;

class FindCommand : public Command {
public:
	FindCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~FindCommand();

	virtual void Execute();
private:
	CFindReplaceDialog* findReplaceDialog;
};

#endif // !_FINDCOMMAND_H
