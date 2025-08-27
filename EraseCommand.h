#ifndef _ERASECOMMAND_H //guard
#define _ERASECOMMAND_H
#include "UndoableCommand.h"

typedef signed long int Long;

class CWnd;

class EraseCommand : public UndoableCommand {
public:
	EraseCommand(CWnd* parent);
	virtual ~EraseCommand();
	EraseCommand(const EraseCommand& source);
	EraseCommand& operator=(const EraseCommand& source);

	virtual void Execute();
	virtual void Undo();
	Command* Clone();
private:
	char character[2];
	Long offset;
};


#endif // !_ERASECOMMAND_H
