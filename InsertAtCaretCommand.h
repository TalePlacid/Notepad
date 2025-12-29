#ifndef _INSERTATCARETCOMMAND_H //guard
#define _INSERTATCARETCOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class InsertAtCaretCommand : public Command {
public:
	InsertAtCaretCommand(CWnd* parent, char(*character), BOOL onChar = TRUE);
	virtual ~InsertAtCaretCommand();
	InsertAtCaretCommand(const InsertAtCaretCommand& source);
	InsertAtCaretCommand& operator=(const InsertAtCaretCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual UINT GetId();
	virtual bool IsUndoable();
private:
	char character[2];
	UINT onChar;
	Long offset;
	Long columnIndex;
};

#endif // !_INSERTATCARETCOMMAND_H
