#ifndef _COMMANDFACTORY_H //guard
#define _COMMANDFACTORY_H
#include <afxwin.h>

class Command;

class CommandFactory {
public:
	CommandFactory();
	~CommandFactory();

	Command* Create(CWnd* parent, UINT nID);
};

#endif // !_COMMANDFACTORY_H
