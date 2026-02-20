#ifndef _WRITEATENDCOMMAND_H //guard
#define _WRITEATENDCOMMAND_H
#include <afxwin.h>
#include "Command.h"
#include "../resource.h"

typedef signed long int Long;

class WriteAtEndCommand : public Command {
public:
	WriteAtEndCommand(CWnd* parent, const TCHAR(*character), BOOL isCompositing = FALSE);
	virtual ~WriteAtEndCommand();
	WriteAtEndCommand(const WriteAtEndCommand& source);
	WriteAtEndCommand& operator=(const WriteAtEndCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual AppID GetID();
	virtual bool IsUndoable();
private:
	char character[2];
	BOOL isCompositing;
	Long offset;
};

inline bool WriteAtEndCommand::IsUndoable() {
	return true;
}

#endif // !_WRITEATENDCOMMAND_H
