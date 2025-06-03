#ifndef _PASTECOMMAND_H //guard
#define _PASTECOMMAND_H
#include "Command.h"

typedef signed long int Long;

class CWnd;

class PasteCommand : public Command {
public:
	PasteCommand(CWnd* parent);
	virtual ~PasteCommand();

	virtual void Execute();
};

#endif // !_PASTECOMMAND_H
