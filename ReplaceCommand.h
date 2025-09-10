#ifndef _REPLACECOMMAND_H //guard
#define _REPLACECOMMAND_H
#include <afxwin.h>
#include "FindReplaceCommand.h"

class CFindReplaceDialog;

class ReplaceCommand : public FindReplaceCommand {
public:
	ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceCommand();

	virtual void Execute();
	virtual void Unexecute();
	virtual Command* Clone();
	virtual UINT GetId();
};

#endif // !_REPLACECOMMAND_H
