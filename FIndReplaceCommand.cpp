#include <afxwin.h>
#include <afxdlgs.h>
#include "FindReplaceCommand.h"

#pragma warning(disable:4996)

FindReplaceCommand::FindReplaceCommand(CWnd* parent, CFindReplaceDialog* findReplaceDialog)
	:Command(parent) {
	this->findReplaceDialog = findReplaceDialog;
}

FindReplaceCommand::~FindReplaceCommand() {

}