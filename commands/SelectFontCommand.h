#ifndef _SELECTFONTCOMMAND_H //guard
#define _SELECTFONTCOMMAND_H
#include <afxwin.h>
#include "Command.h"

class Font;

class SelectFontCommand : public Command {
public:
	SelectFontCommand(CWnd* parent);
	virtual ~SelectFontCommand();

	virtual void Execute();
};

#endif // !_SELECTFONTCOMMAND_H