#ifndef _WRITEATENDCOMMAND_H //guard
#define _WRITEATENDCOMMAND_H
#include <afxwin.h>
#include "UndoableCommand.h"

typedef signed long int Long;

class WriteAtEndCommand : public UndoableCommand {
public:
	WriteAtEndCommand(CWnd* parent, char(*character), BOOL onChar = TRUE);
	virtual ~WriteAtEndCommand();
	WriteAtEndCommand(const WriteAtEndCommand& source);
	WriteAtEndCommand& operator=(const WriteAtEndCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual Command* Clone();
	virtual UINT GetId();
private:
	char character[2];
	BOOL onChar;
	Long offset;
};


#endif // !_WRITEATENDCOMMAND_H
