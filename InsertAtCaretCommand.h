#ifndef _INSERTATCARETCOMMAND_H //guard
#define _INSERTATCARETCOMMAND_H
#include "Command.h"

typedef signed long int Long;

class CWnd;

class InsertAtCaretCommand : public Command {
public:
	InsertAtCaretCommand(CWnd* parent, char(*character));
	virtual ~InsertAtCaretCommand();
	InsertAtCaretCommand(const InsertAtCaretCommand& source);
	InsertAtCaretCommand& operator=(const InsertAtCaretCommand& source);

	virtual void Execute();
private:
	char character[2];
};

#endif // !_INSERTATCARETCOMMAND_H
