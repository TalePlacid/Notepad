#ifndef _REPLACEALLCOMMAND_H
#define _REPLACEALLCOMMAND_H
#include "../commands/MacroCommand.h"

class Command;
class CWnd;
class CFindReplaceDialog;
class FindReplaceOption;

class ReplaceAllCommand : public MacroCommand {
public:
	ReplaceAllCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~ReplaceAllCommand();

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();

	virtual bool IsUndoable();
	virtual CString GetSource();
	virtual CString GetReplaced();
	virtual FindReplaceOption* GetFindReplaceOption() ;
private:
	CFindReplaceDialog* findReplaceDialog;
};

#endif // !_REPLACEALLCOMMAND_H
