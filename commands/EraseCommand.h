#ifndef _ERASECOMMAND_H //guard
#define _ERASECOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class EraseCommand : public Command {
public:
	EraseCommand(CWnd* parent, BOOL onChar = TRUE);
	virtual ~EraseCommand();
	EraseCommand(const EraseCommand& source);
	EraseCommand& operator=(const EraseCommand& source);

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

inline AppID EraseCommand::GetID() {
	return AppID::ID_COMMAND_ERASE;
}

#endif // !_ERASECOMMAND_H

