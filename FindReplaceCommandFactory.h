#ifndef _FINDREPLACECOMMANDFACTORY_H //guard
#define _FINDREPLACECOMMANDFACTORY_H
#include <afxwin.h>

class Command;
class CFindReplaceDialog;

class FindReplaceCommandFactory {
public:
	FindReplaceCommandFactory();
	~FindReplaceCommandFactory();

	Command* Create(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
};


#endif // !_FINDREPLACECOMMANDFACTORY_H
