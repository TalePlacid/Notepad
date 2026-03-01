#include "ActionFactory.h"
#include "../FindReplaceOption.h"
#include "NewAction.h"
#include "NewWindowAction.h"
#include "OpenAction.h"
#include "SaveAction.h"
#include "SaveAsAction.h"
#include "SetPageAction.h"
#include "PreviewAction.h"
#include "PrintAction.h"
#include "ChangeFontAction.h"
#include "AutoWrapAction.h"
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
#include "MovePageUpAction.h"
#include "MovePageDownAction.h"
#include "MovePageLeftAction.h"
#include "MovePageRightAction.h"
#include "SelectUpAction.h"
#include "SelectDownAction.h"
#include "SelectLeftAction.h"
#include "SelectRightAction.h"
#include "SelectLineFrontAction.h"
#include "SelectLineRearAction.h"
#include "SelectWordLeftAction.h"
#include "SelectWordRightAction.h"
#include "SelectDocFrontAction.h"
#include "SelectDocRearAction.h"
#include "SelectPageUpAction.h"
#include "SelectPageDownAction.h"
#include "SelectAllAction.h"
#include "CopyAction.h"
#include "OpenFindDialogAction.h"
#include "OpenReplaceDialogAction.h"
#include "FindAction.h"
#include "FindPreviousAction.h"
#include "FindNextAction.h"
#include "CloseFindReplaceAction.h"
#include "ZoomInAction.h"
#include "ZoomOutAction.h"
#include "ZoomResetAction.h"
#include "UndoAction.h"
#include "RedoAction.h"
#include "AboutNotepadAction.h"

#pragma warning(disable:4996)

ActionFactory::ActionFactory() {

}

ActionFactory::~ActionFactory() {

}

Action* ActionFactory::Create(CWnd* parent, AppID nID, FindReplaceOption* findReplaceOption) {
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
	case AppID::ID_ACTION_SET_PAGE:
		action = new SetPageAction(parent);
		break;
	case AppID::ID_ACTION_PREVIEW:
		action = new PreviewAction(parent);
		break;
	case AppID::ID_ACTION_PRINT:
		action = new PrintAction(parent);
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
	case AppID::ID_ACTION_MOVE_PAGE_UP:
		action = new MovePageUpAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_PAGE_DOWN:
		action = new MovePageDownAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_PAGE_LEFT:
		action = new MovePageLeftAction(parent);
		break;
	case AppID::ID_ACTION_MOVE_PAGE_RIGHT:
		action = new MovePageRightAction(parent);
		break;	
	case AppID::ID_ACTION_SELECT_UP:
		action = new SelectUpAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_DOWN:
		action = new SelectDownAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_LEFT:
		action = new SelectLeftAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_RIGHT:
		action = new SelectRightAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_LINE_FRONT:
		action = new SelectLineFrontAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_LINE_REAR:
		action = new SelectLineRearAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_WORD_LEFT:
		action = new SelectWordLeftAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_WORD_RIGHT:
		action = new SelectWordRightAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_DOC_FRONT:
		action = new SelectDocFrontAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_DOC_REAR:
		action = new SelectDocRearAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_PAGE_UP:
		action = new SelectPageUpAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_PAGE_DOWN:
		action = new SelectPageDownAction(parent);
		break;
	case AppID::ID_ACTION_SELECT_ALL:
		action = new SelectAllAction(parent);
		break;
	case AppID::ID_ACTION_COPY:
		action = new CopyAction(parent);
		break;
	case AppID::ID_ACTION_OPEN_FIND_DIALOG:
		action = new OpenFindDialogAction(parent);
		break;
	case AppID::ID_ACTION_OPEN_REPLACE_DIALOG:
		action = new OpenReplaceDialogAction(parent);
		break;
	case AppID::ID_ACTION_FIND:
		action = new FindAction(parent, *findReplaceOption);
		break;
	case AppID::ID_ACTION_FIND_PREVIOUS:
		action = new FindPreviousAction(parent);
		break;
	case AppID::ID_ACTION_FIND_NEXT:
		action = new FindNextAction(parent);
		break;
	case AppID::ID_ACTION_CLOSE_FIND_REPLACE_DIALOG:
		action = new CloseFindReplaceAction(parent);
		break;
	case AppID::ID_ACTION_ZOOM_IN:
		action = new ZoomInAction(parent);
		break;
	case AppID::ID_ACTION_ZOOM_OUT:
		action = new ZoomOutAction(parent);
		break;
	case AppID::ID_ACTION_ZOOM_RESET:
		action = new ZoomResetAction(parent);
		break;
	case AppID::ID_ACTION_UNDO:
		action = new UndoAction(parent);
		break;
	case AppID::ID_ACTION_REDO:
		action = new RedoAction(parent);
		break;
	case AppID::ID_ACTION_ABOUT_NOTEPAD:
		action = new AboutNotepadAction(parent);
		break;
	default:
		break;
	}

	return action;
}