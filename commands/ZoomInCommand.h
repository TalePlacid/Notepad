#ifndef _ZOOMINCOMMAND_H //guard
#define _ZOOMINCOMMAND_H
#include "Command.h"

class ZoomInCommand : public Command {
public:
	ZoomInCommand(CWnd* parent);
	virtual ~ZoomInCommand();

	virtual void Execute();
};

#endif // !_ZOOMINCOMMAND_H
