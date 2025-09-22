#ifndef _WRITEATENDCOMMAND_H //guard
#define _WRITEATENDCOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class WriteAtEndCommand : public Command {
public:
	WriteAtEndCommand(CWnd* parent, char(*character), BOOL onChar = TRUE);
	virtual ~WriteAtEndCommand();
	WriteAtEndCommand(const WriteAtEndCommand& source);
	WriteAtEndCommand& operator=(const WriteAtEndCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual UINT GetId();
	virtual bool IsUndoable();
private:
	char character[2];
	BOOL onChar;
	Long offset;
};

#endif // !_WRITEATENDCOMMAND_H
