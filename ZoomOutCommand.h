#ifndef _ZOOMOUTCOMMAND_H //guard
#define _ZOOMOUTCOMMAND_H
#include "Command.h"

class ZoomOutCommand : public Command {
public:
	ZoomOutCommand(CWnd* parent);
	virtual ~ZoomOutCommand();

	virtual void Execute();
};

#endif // !_ZOOMOUTCOMMAND_H
