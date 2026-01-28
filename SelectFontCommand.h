#ifndef _SELECTFONTCOMMAND_H //guard
#define _SELECTFONTCOMMAND_H
#include "Command.h"
#include <afxwin.h>

class Font;

class SelectFontCommand : public Command {
public:
	SelectFontCommand(CWnd* parent);
	virtual ~SelectFontCommand();

	virtual void Execute();
};

#endif // !_SELECTFONTCOMMAND_H