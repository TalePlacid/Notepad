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
	virtual bool NeedsNoteTruncation();
private:
	char character[2];
	BOOL onChar;
	Long offset;
	Long columnIndex;
	BOOL isErased;
	Long erasedFrontOffset;
	Long erasedRearOffset;
	Long erasedColumnIndex;
	CString erasedContents;
};

inline AppID WriteAtEndCommand::GetID() {
	return AppID::ID_COMMAND_WRITE_AT_END;
}

inline bool WriteAtEndCommand::NeedsNoteTruncation() {
	return true;
}

#endif // !_WRITEATENDCOMMAND_H

