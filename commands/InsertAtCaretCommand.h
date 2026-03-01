#ifndef _INSERTATCARETCOMMAND_H //guard
#define _INSERTATCARETCOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class InsertAtCaretCommand : public Command {
public:
	InsertAtCaretCommand(CWnd* parent, const TCHAR(*character), BOOL onChar = TRUE);
	virtual ~InsertAtCaretCommand();
	InsertAtCaretCommand(const InsertAtCaretCommand& source);
	InsertAtCaretCommand& operator=(const InsertAtCaretCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual AppID GetID();
private:
	char character[2];
	UINT onChar;
	Long offset;
	Long columnIndex;
	BOOL isErased;
	Long erasedFrontOffset;
	Long erasedRearOffset;
	Long erasedColumnIndex;
	CString erasedContents;
};

inline AppID InsertAtCaretCommand::GetID() {
	return AppID::ID_COMMAND_INSERT_AT_CARET;
}

#endif // !_INSERTATCARETCOMMAND_H

