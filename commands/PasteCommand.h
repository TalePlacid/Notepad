#ifndef _PASTECOMMAND_H //guard
#define _PASTECOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class Command;

class PasteCommand : public Command {
public:
	PasteCommand(CWnd* parent);
	virtual ~PasteCommand();
	PasteCommand(const PasteCommand& source);
	PasteCommand& operator=(const PasteCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual AppID GetID();
private:
	Long frontOffset;
	Long rearOffset;
	Long columnIndex;
	CString erased;
	CString contents;
};

inline AppID PasteCommand::GetID() {
	return AppID::ID_COMMAND_PASTE;
}

#endif // !_PASTECOMMAND_H
