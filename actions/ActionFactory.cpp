#include "ActionFactory.h"
#include "NewAction.h"
#include "NewWindowAction.h"
#include "OpenAction.h"
#include "SaveAction.h"
#include "SaveAsAction.h"
#include "ChangeFontAction.h"
#include "MoveUpAction.h"
#include "MoveDownAction.h"
#include "MoveLeftAction.h"
#include "MoveRightAction.h"
#include "MoveLineFrontAction.h"
#include "MoveLineRearAction.h"
#include "MoveWordRightAction.h"
#include "MoveWordLeftAction.h"
#include "MoveDocFrontAction.h"
#include "MoveDocRearAction.h"
#include "AutoWrapAction.h"

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
	case AppID::ID_ACTION_AUTOWRAP:
		action = new AutoWrapAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_UP:
		action = new MoveUpAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_DOWN:
		action = new MoveDownAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_LEFT:
		action = new MoveLeftAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_RIGHT:
		action = new MoveRightAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_LINE_FRONT:
		action = new MoveLineFrontAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_LINE_REAR:
		action = new MoveLineRearAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_WORD_RIGHT:
		action = new MoveWordRightAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_WORD_LEFT:
		action = new MoveWordLeftAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_DOC_FRONT:
		action = new MoveDocFrontAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_DOC_REAR:
		action = new MoveDocRearAction(parent);
		break;
	default:
		break;
	}

	return action;
}