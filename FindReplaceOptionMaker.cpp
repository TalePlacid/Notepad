#include <afxdlgs.h>
#include "FindReplaceOptionMaker.h"

#pragma warning(disable:4996)

FindReplaceOptionMaker::FindReplaceOptionMaker() {

}

FindReplaceOptionMaker::~FindReplaceOptionMaker() {

}

FindReplaceOption FindReplaceOptionMaker::Make(CFindReplaceDialog* findReplaceDialog) {
	FindReplaceOption findReplaceOption;
	findReplaceOption.findString = findReplaceDialog->GetFindString();
	findReplaceOption.replaceString = findReplaceDialog->GetReplaceString();
	findReplaceOption.isMatchCase = findReplaceDialog->MatchCase();
	findReplaceOption.isMatchWhole = findReplaceDialog->MatchWholeWord();
	findReplaceOption.isSearchDown = findReplaceDialog->SearchDown();

	return findReplaceOption;
}
