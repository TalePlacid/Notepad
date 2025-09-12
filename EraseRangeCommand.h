#ifndef _ERASERANGECOMMAND_H //guard
#define _ERASERANGECOMMAND_H
#include <afxwin.h>
#include "Command.h"

typedef signed long int Long;

class EraseRangeCommand : public Command {
public:
	EraseRangeCommand(CWnd* parent);
	virtual ~EraseRangeCommand();
	EraseRangeCommand(const EraseRangeCommand& source);
	EraseRangeCommand& operator=(const EraseRangeCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual Command* Clone();
	virtual UINT GetId();
	virtual bool IsUndoable();
private:
	Long frontOffset;
	Long rearOffset;
	CString contents;
};

#endif // !_ERASERANGECOMMAND_H
