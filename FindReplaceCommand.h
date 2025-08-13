#ifndef _FINDREPLACECOMMAND_H //guard
#define _FINDREPLACECOMMAND_H
#include "Command.h"

class CWnd;
class CFindReplaceDialog;

class FindReplaceCommand : public Command {
public:
	FindReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~FindReplaceCommand() = 0;
protected:
	CFindReplaceDialog* findReplaceDialog;
};

#endif // !_FINDREPLACECOMMAND_H
