#ifndef _ERASECOMMAND_H //guard
#define _ERASECOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class EraseCommand : public Command {
public:
	EraseCommand(CWnd* parent);
	virtual ~EraseCommand();
	EraseCommand(const EraseCommand& source);
	EraseCommand& operator=(const EraseCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual Command* Clone();
	virtual UINT GetId();
	virtual bool IsUndoable();
private:
	char character[2];
	Long offset;
};

#endif // !_ERASECOMMAND_H
