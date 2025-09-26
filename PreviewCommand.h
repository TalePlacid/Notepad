#ifndef _PREVIEWCOMMAND_H //guard
#define _PREVIEWCOMMAND_H
#include <afxwin.h>
#include "Command.h"

class PreviewCommand : public Command {
public:
	PreviewCommand(CWnd* parent);
	virtual ~PreviewCommand();

	virtual void Execute();
};


#endif // !_PREVIEWCOMMAND_H
