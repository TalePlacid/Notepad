#ifndef _COMMANDFACTORY_H //guard
#define _COMMANDFACTORY_H
#include <afxwin.h>

class Command;
class CFindReplaceDialog;

class CommandFactory {
public:
	CommandFactory();
	~CommandFactory();

	Command* Create(CWnd* parent, UINT nID, CFindReplaceDialog* findReplaceDialog = NULL);
};

#endif // !_COMMANDFACTORY_H
