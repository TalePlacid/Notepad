#ifndef _ERASEBEFORECARETCOMMAND_H //guard
#define _ERASEBEFORECARETCOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class EraseBeforeCaretCommand : public Command {
public:
	EraseBeforeCaretCommand(CWnd* parent, BOOL onChar = TRUE);
	virtual ~EraseBeforeCaretCommand();
	EraseBeforeCaretCommand(const EraseBeforeCaretCommand& source);
	EraseBeforeCaretCommand& operator=(const EraseBeforeCaretCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual AppID GetID();
private:
	char character[2];
	BOOL onChar;
	Long offset;
	Long columnIndex;
};

inline AppID EraseBeforeCaretCommand::GetID() {
	return AppID::ID_COMMAND_ERASE_BEFORE_CARET;
}

#endif // !_ERASEBEFORECARETCOMMAND_H

