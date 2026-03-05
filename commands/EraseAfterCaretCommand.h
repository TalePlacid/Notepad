#ifndef _ERASEAFTERCARETCOMMAND_H //guard
#define _ERASEAFTERCARETCOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class EraseAfterCaretCommand : public Command {
public:
	EraseAfterCaretCommand(CWnd* parent, BOOL onChar = TRUE);
	virtual ~EraseAfterCaretCommand();
	EraseAfterCaretCommand(const EraseAfterCaretCommand& source);
	EraseAfterCaretCommand& operator=(const EraseAfterCaretCommand& source);

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

inline AppID EraseAfterCaretCommand::GetID() {
	return AppID::ID_COMMAND_ERASE_AFTER_CARET;
}

#endif // !_ERASEAFTERCARETCOMMAND_H