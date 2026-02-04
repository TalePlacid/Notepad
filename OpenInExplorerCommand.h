#ifndef _OPENINEXPLORERCOMMAND_H //guard
#define _OPENINEXPLORERCOMMAND_H
#include <afxwin.h>
#include "Command.h"

class OpenInExplorerCommand : public Command {
public:
	OpenInExplorerCommand(CWnd* parent);
	virtual ~OpenInExplorerCommand();

	virtual void Execute();
};

#endif // !_OPENINEXPLORERCOMMAND_H
