#ifndef _REPLACEALLCOMMAND_H
#define _REPLACEALLCOMMAND_H
#include "FindReplaceCommand.h"

class CWnd;
class CFindReplaceDialog;

class ReplaceAllCommand : public FindReplaceCommand {
public:
	ReplaceAllCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceAllCommand();

	virtual void Execute();
};

#endif // !_REPLACEALLCOMMAND_H
