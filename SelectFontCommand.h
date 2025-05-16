#ifndef _SELECTFONTCOMMAND_H //guard
#define _SELECTFONTCOMMAND_H
#include "Command.h"
#include <afxwin.h>

class Font;

class SelectFontCommand : public Command {
public:
	SelectFontCommand(CWnd* parent);
	virtual ~SelectFontCommand();
	SelectFontCommand(const SelectFontCommand& source);
	SelectFontCommand& operator=(const SelectFontCommand& source);

	virtual void Execute();
	virtual void Unexecute();
private:
	Font* oldFont;
};

#endif // !_SELECTFONTCOMMAND_H