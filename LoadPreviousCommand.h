#ifndef _LOADPREVIOUSCOMMAND_H
#define _LOADPREVIOUSCOMMAND_H
#include "Command.h"

class CWnd;

class LoadPreviousCommand : public Command {
public:
	LoadPreviousCommand(CWnd* parent);
	virtual ~LoadPreviousCommand();
	
	virtual void Execute();
	virtual bool NeedScrollBarUpdate();
};

inline bool LoadPreviousCommand::NeedScrollBarUpdate() {
	return false;
}

#endif // !_LOADPREVIOUSCOMMAND_H
