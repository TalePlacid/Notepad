#include <afxwin.h>
#include "FindAction.h"
#include "../Editor.h"

#pragma warning(disable:4996)

FindAction::FindAction(CWnd* parent, FindReplaceOption findReplaceOption)
	:Action(parent) {
	this->findReplaceOption = findReplaceOption;
}

FindAction::~FindAction() {

}

void FindAction::Perform() {
	Editor editor(this->parent);
	editor.Find(findReplaceOption);
}
