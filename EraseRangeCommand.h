#ifndef _ERASERANGECOMMAND_H //guard
#define _ERASERANGECOMMAND_H
#include <afxwin.h>
#include "UndoableCommand.h"

typedef signed long int Long;

class EraseRangeCommand : public UndoableCommand {
public:
	EraseRangeCommand(CWnd* parent);
	virtual ~EraseRangeCommand();
	EraseRangeCommand(const EraseRangeCommand& source);
	EraseRangeCommand& operator=(const EraseRangeCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual Command* Clone();
	virtual UINT GetId();
private:
	Long frontOffset;
	Long rearOffset;
	CString contents;
};

#endif // !_ERASERANGECOMMAND_H
