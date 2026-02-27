#ifndef _REPLACECOMMAND_H //guard
#define _REPLACECOMMAND_H
#include "Command.h"
#include "../AppID.h"
#include "../FindReplaceOption.h"

class CWnd;

class ReplaceCommand : public Command {
public:
	ReplaceCommand(CWnd* parent, FindReplaceOption findReplaceOption);
	virtual ~ReplaceCommand();
	ReplaceCommand(const ReplaceCommand& source);
	ReplaceCommand& operator=(const ReplaceCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();

	virtual Command* Clone();
	virtual AppID GetID();

	virtual bool ShouldKeepSelection();
private:
	FindReplaceOption findReplaceOption;
	Long columnIndex;
	Long offset;
	CString source;
	CString replaced;
};

inline bool ReplaceCommand::ShouldKeepSelection() {
	return true;
}

inline AppID ReplaceCommand::GetID() {
	return AppID::ID_COMMAND_REPLACE;
}

#endif // !_REPLACECOMMAND_H
