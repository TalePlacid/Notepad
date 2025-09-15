#ifndef _REPLACECOMMAND_H //guard
#define _REPLACECOMMAND_H
#include <afxwin.h>
#include "FindReplaceCommand.h"

class CFindReplaceDialog;

class ReplaceCommand : public FindReplaceCommand {
public:
	ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceCommand();
	ReplaceCommand(const ReplaceCommand& source);
	ReplaceCommand& operator=(const ReplaceCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual Command* Clone();
	virtual UINT GetId();
	virtual bool IsUndoable();
private:
	Long offset;
	CString source;
	CString replaced;
};


#endif // !_REPLACECOMMAND_H
