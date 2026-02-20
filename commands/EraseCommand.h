#ifndef _ERASECOMMAND_H //guard
#define _ERASECOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class EraseCommand : public Command {
public:
	EraseCommand(CWnd* parent, BOOL isCompositing = FALSE);
	virtual ~EraseCommand();
	EraseCommand(const EraseCommand& source);
	EraseCommand& operator=(const EraseCommand& source);

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
	Long columnIndex;
};

#endif // !_ERASECOMMAND_H
