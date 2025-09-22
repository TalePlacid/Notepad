#ifndef _REPLACEALLCOMMAND_H
#define _REPLACEALLCOMMAND_H
#include "MacroCommand.h"

class Command;
class CWnd;
class CFindReplaceDialog;

class ReplaceAllCommand : public MacroCommand {
public:
	ReplaceAllCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceAllCommand();

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual void Update(Command* command, bool isDone);
	virtual CString GetSource();
	virtual CString GetReplaced();
private:
	CFindReplaceDialog* findReplaceDialog;
};

#endif // !_REPLACEALLCOMMAND_H
