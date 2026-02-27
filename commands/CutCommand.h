#ifndef _CUTCOMMAND_H //guard
#define _CUTCOMMAND_H
#include "Command.h"

class CWnd;

class CutCommand : public Command {
public:
	CutCommand(CWnd* parent);
	virtual ~CutCommand();
	CutCommand(const CutCommand& source);
	CutCommand& operator=(const CutCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();

	virtual AppID GetID();
private:
	Long frontOffset;
	Long rearOffset;
	Long columnIndex;
	CString contents;
};

inline AppID CutCommand::GetID() {
	return AppID::ID_COMMAND_CUT;
}

#endif // !_CUTCOMMAND_H
