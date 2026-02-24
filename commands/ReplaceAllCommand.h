#ifndef _REPLACEALLCOMMAND_H //guard
#define _REPLACEALLCOMMAND_H
#include "Command.h"
#include "../AppID.h"
#include "../FindReplaceOption.h"

class CWnd;

class ReplaceAllCommand : public Command {
public:
	ReplaceAllCommand(CWnd* parent, FindReplaceOption findReplaceOption);
	virtual ~ReplaceAllCommand();
	ReplaceAllCommand(const ReplaceAllCommand& source);
	ReplaceAllCommand& operator=(const ReplaceAllCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();

	virtual Command* Clone();
	virtual AppID GetID();
	virtual bool ShouldKeepSelection();
private:
	FindReplaceOption findReplaceOption;
	Long columnIndex;
	Long(*offsets);
	Long length;
	CString source;
	CString replaced;
};

inline bool ReplaceAllCommand::ShouldKeepSelection() {
	return true;
}

#endif // !_REPLACEALLCOMMAND_H
