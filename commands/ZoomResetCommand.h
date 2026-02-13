#ifndef _ZOOMRESETCOMMAND_H //guard
#define _ZOOMRESETCOMMAND_H
#include "Command.h"

class ZoomResetCommand : public Command {
public:
	ZoomResetCommand(CWnd* parent);
	virtual ~ZoomResetCommand();

	virtual void Execute();
};

#endif // !_ZOOMRESETCOMMAND_H
