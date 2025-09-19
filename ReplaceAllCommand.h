#ifndef _REPLACEALLCOMMAND_H
#define _REPLACEALLCOMMAND_H
#include "MacroCommand.h"

class CWnd;
class CFindReplaceDialog;

class ReplaceAllCommand : public MacroCommand {
public:
	ReplaceAllCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceAllCommand();

	virtual void Execute();
private:
	CFindReplaceDialog* findReplaceDialog;
};

#endif // !_REPLACEALLCOMMAND_H
