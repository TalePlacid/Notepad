#ifndef _PASTECOMMAND_H //guard
#define _PASTECOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class Command;

class PasteCommand : public Command {
public:
	PasteCommand(CWnd* parent);
	virtual ~PasteCommand();
	PasteCommand(const PasteCommand& source);
	PasteCommand& operator=(const PasteCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual AppID GetID();
	virtual bool IsUndoable();
private:
	Long offset;
	Long columnIndex;
	CString contents;
	CString erased;
};

#endif // !_PASTECOMMAND_H
