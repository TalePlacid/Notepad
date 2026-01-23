#ifndef _PRINTCOMMAND_H //guard
#define _PRINTCOMMAND_H
#include "Command.h"

class CWnd;
class PreviewPaginator;

class PrintCommand : public Command {
public:
	PrintCommand(CWnd* parent);
	virtual ~PrintCommand();

	virtual void Execute();
private:
	PreviewPaginator* previewPaginator;
};

#endif // !_PRINTCOMMAND_H
