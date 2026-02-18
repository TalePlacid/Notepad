#include "ActionFactory.h"
#include "NewAction.h"
#include "NewWindowAction.h"
#include "OpenAction.h"
#include "SaveAction.h"
#include "SaveAsAction.h"
#include "ChangeFontAction.h"

#pragma warning(disable:4996)

ActionFactory::ActionFactory() {

}

ActionFactory::~ActionFactory() {

}

Action* ActionFactory::Create(CWnd* parent, AppID nID) {
	Action* action = NULL;

	switch (nID)
	{
	case AppID::ID_ACTION_NEW:
		action = new NewAction(parent);
		break;
	case AppID::ID_ACTION_NEW_WINDOW:
		action = new NewWindowAction(parent);
		break;
	case AppID::ID_ACTION_OPEN:
		action = new OpenAction(parent);
		break;
	case AppID::ID_ACTION_SAVE:
		action = new SaveAction(parent);
		break;
	case AppID::ID_ACTION_SAVE_AS:
		action = new SaveAsAction(parent);
		break;
	case AppID::ID_ACTION_CHANGE_FONT:
		action = new ChangeFontAction(parent);
		break;
	default:
		break;
	}

	return action;
}