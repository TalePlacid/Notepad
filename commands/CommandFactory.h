#ifndef _COMMANDFACTORY_H //guard
#define _COMMANDFACTORY_H
#include <afxwin.h>
#include "../AppID.h"

class Command;
class FindReplaceOption;

class CommandFactory {
public:
	CommandFactory();
	~CommandFactory();

	static Command* Create(CWnd* parent, AppID nID, const TCHAR(*character) = 0,
		BOOL isCompositing = FALSE, BOOL isSelected = FALSE,
		FindReplaceOption* findReplaceOption = 0);
};

#endif // !_COMMANDFACTORY_H
