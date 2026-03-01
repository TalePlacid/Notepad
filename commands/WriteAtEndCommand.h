#ifndef _WRITEATENDCOMMAND_H //guard
#define _WRITEATENDCOMMAND_H
#include <afxwin.h>
#include "Command.h"
#include "../resource.h"

typedef signed long int Long;

class WriteAtEndCommand : public Command {
public:
	WriteAtEndCommand(CWnd* parent, const TCHAR(*character), BOOL onChar = TRUE);
	virtual ~WriteAtEndCommand();
	WriteAtEndCommand(const WriteAtEndCommand& source);
	WriteAtEndCommand& operator=(const WriteAtEndCommand& source);

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

inline AppID WriteAtEndCommand::GetID() {
	return AppID::ID_COMMAND_WRITE_AT_END;
}

#endif // !_WRITEATENDCOMMAND_H

