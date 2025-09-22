#ifndef _REPLACECOMMAND_H //guard
#define _REPLACECOMMAND_H
#include <afxwin.h>
#include "Command.h"

class CFindReplaceDialog;

class ReplaceCommand : public Command {
public:
	ReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceCommand();
	ReplaceCommand(const ReplaceCommand& source);
	ReplaceCommand& operator=(const ReplaceCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual UINT GetId();
	virtual bool IsUndoable();
	virtual void Update(Command* command, bool isDone);
	virtual Long GetOffset();
	virtual CString GetSource();
	virtual CString GetReplaced();
private:
	CFindReplaceDialog* findReplaceDialog;
	Long offset;
	CString source;
	CString replaced;
};


#endif // !_REPLACECOMMAND_H
