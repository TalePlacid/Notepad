#ifndef _REPLACECOMMAND_H //guard
#define _REPLACECOMMAND_H
#include "FindReplaceCommand.h"

class CWnd;
class CFindReplaceDialog;

class ReplaceCommand : public FindReplaceCommand {
public:
	ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceCommand();

	virtual void Execute();
};

#endif // !_REPLACECOMMAND_H
