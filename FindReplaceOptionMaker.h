#ifndef _FINDREPLACEOPTIONMAKER_H //guard
#define _FINDREPLACEOPTIONMAKER_H
#include "FindReplaceOption.h"

class CFindReplaceDialog;

class FindReplaceOptionMaker {
public:
	FindReplaceOptionMaker();
	~FindReplaceOptionMaker();

	static FindReplaceOption Make(CFindReplaceDialog* findReplaceDialog);
};

#endif // !_FINDREPLACEOPTIONMAKER_H
