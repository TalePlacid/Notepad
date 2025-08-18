#ifndef _FINDREPLACECOMMANDFACTORY_H //guard
#define _FINDREPLACECOMMANDFACTORY_H
#include <afxwin.h>

class Command;
class CFindReplaceDialog;

struct FindReplaceOption {
	CString findString;
	BOOL isMatchWhole;
	BOOL isMatchCase;
	BOOL isSearchDown;

	FindReplaceOption(CString findString, BOOL isMatchWhole, BOOL isMatchCase, BOOL isSearchDown) {
		this->findString = findString;
		this->isMatchWhole = isMatchWhole;
		this->isMatchCase = isMatchCase;
		this->isSearchDown = isSearchDown;
	}

	BOOL operator==(FindReplaceOption& other) {
		return (this->findString == other.findString)
			&& (this->isMatchWhole == other.isMatchWhole)
			&& (this->isMatchCase == other.isMatchCase)
			&& (this->isSearchDown == other.isSearchDown);
	}

	BOOL operator!=(FindReplaceOption& other) {
		return (this->findString != other.findString)
			|| (this->isMatchWhole != other.isMatchWhole)
			|| (this->isMatchCase != other.isMatchCase)
			|| (this->isSearchDown != other.isSearchDown);
	}
};

class FindReplaceCommandFactory {
public:
	FindReplaceCommandFactory();
	~FindReplaceCommandFactory();

	Command* Create(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
};


#endif // !_FINDREPLACECOMMANDFACTORY_H
