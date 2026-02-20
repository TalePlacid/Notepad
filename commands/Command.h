#ifndef _COMMAND_H //guard
#define _COMMAND_H
#include <afxwin.h>
#include "../AppID.h"

typedef signed long int Long;

class FindReplaceOption;

class Command {
public:
	Command(CWnd* parent = NULL);
	virtual ~Command() = 0;
	Command(const Command& source);
	Command& operator=(const Command& source);

	virtual void Execute() = 0;
	virtual void Undo();
	virtual void Redo();
	virtual Command* Clone();
	virtual Long Add(Command* command);
	virtual Command* GetAt(Long index);

	virtual bool IsUndoable();
	virtual bool IsMacroCommand();
	virtual bool NeedScrollBarUpdate();

	virtual AppID GetID();
	virtual Long GetOffset();
	virtual CString GetSource();
	virtual CString GetReplaced();
	virtual CWnd* GetParent() const;
	virtual Long GetLength() const;
	virtual FindReplaceOption* GetFindReplaceOption();
protected:
	CWnd* parent;
};

inline CWnd* Command::GetParent() const {
	return const_cast<CWnd*>(this->parent);
}

inline Long Command::GetLength() const {
	return 0;
}

inline bool Command::NeedScrollBarUpdate() {
	return true;
}

inline FindReplaceOption* Command::GetFindReplaceOption() {
	return 0;
}

#endif // !_COMMAND_H