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
	virtual UINT GetId();
	virtual bool IsUndoable();
private:
	Long offset;
	CString contents;
};

#endif // !_PASTECOMMAND_H
