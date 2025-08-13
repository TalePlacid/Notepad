#ifndef _FINDNEXTCOMMAND_H //guard
#define _FINDNEXTCOMMAND_H
#include "FindReplaceCommand.h"

class CWnd;
class CFindReplaceDialog;

class FindNextCommand : public FindReplaceCommand {
public:
	FindNextCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~FindNextCommand();

	virtual void Execute();
};

#endif // !_FINDNEXTCOMMAND_H
