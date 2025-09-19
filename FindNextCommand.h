#ifndef _FINDNEXTCOMMAND_H //guard
#define _FINDNEXTCOMMAND_H
#include "Command.h"

class CWnd;
class CFindReplaceDialog;

class FindNextCommand : public Command {
public:
	FindNextCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~FindNextCommand();

	virtual void Execute();
private:
	CFindReplaceDialog* findReplaceDialog;
};

#endif // !_FINDNEXTCOMMAND_H
