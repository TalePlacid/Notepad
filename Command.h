#ifndef _COMMAND_H //guard
#define _COMMAND_H
#include <afxwin.h>

class Command {
public:
	Command(CWnd* parent = NULL);
	virtual ~Command() = 0;
	Command(const Command& source);
	Command& operator=(const Command& source);

	virtual void Execute() = 0;
	virtual void Undo();
	virtual Command* Clone();
	virtual bool IsUndoable();
	virtual UINT GetId();
protected:
	CWnd* parent;
};

#endif // !_COMMAND_H