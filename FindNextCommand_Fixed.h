#ifndef _FINDNEXTCOMMANDFIXED_H //guard
#define _FINDNEXTCOMMANDFIXED_H
#include "FindReplaceCommand.h"

class CWnd;
class CFindReplaceDialog;

class FindNextCommand_Fixed : public FindReplaceCommand {
public:
	FindNextCommand_Fixed(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
	virtual ~FindNextCommand_Fixed();

	virtual void Execute();
};

#endif // !_FINDNEXTCOMMANDFIXED_H
