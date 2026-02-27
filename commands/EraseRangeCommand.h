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
	virtual void Redo();
	virtual Command* Clone();
	virtual AppID GetID();
private:
	Long frontOffset;
	Long rearOffset;
	Long columnIndex;
	CString contents;
};

inline AppID EraseRangeCommand::GetID() {
	return AppID::ID_COMMAND_ERASE_RANGE;
}

#endif // !_ERASERANGECOMMAND_H
