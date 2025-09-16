#ifndef _COMMAND_H //guard
#define _COMMAND_H
#include <afxwin.h>

typedef signed long int Long;

class Command {
public:
	Command(CWnd* parent = NULL);
	virtual ~Command() = 0;
	Command(const Command& source);
	Command& operator=(const Command& source);

	virtual void Execute() = 0;
	virtual void Undo();
	virtual Command* Clone();
	virtual bool IsUndoable();
	virtual Long Add(Command* command);
	virtual Command* GetAt(Long index);
	virtual bool IsMacroCommand();
	virtual UINT GetId();
	virtual Long GetOffset();
	virtual CString GetSource();
	virtual CString GetReplaced();
	virtual void Update(Long difference);

	virtual CWnd* GetParent() const;
protected:
	CWnd* parent;
};

inline CWnd* Command::GetParent() const {
	return const_cast<CWnd*>(this->parent);
}

#endif // !_COMMAND_H