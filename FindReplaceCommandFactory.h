#ifndef _FINDREPLACECOMMANDFACTORY_H //guard
#define _FINDREPLACECOMMANDFACTORY_H
#include <afxwin.h>

class Command;
class CFindReplaceDialog;

struct FindReplaceOption {
	CString findString;
	BOOL isMatchWhole;
	BOOL isMatchCase;

	FindReplaceOption(CString findString, BOOL isMatchWhole, BOOL isMatchCase) {
		this->findString = findString;
		this->isMatchWhole = isMatchWhole;
		this->isMatchCase = isMatchCase;
	}

	BOOL operator==(FindReplaceOption& other) {
		return (this->findString == other.findString) 
			&& (this->isMatchWhole == other.isMatchWhole) 
			&& (this->isMatchCase == other.isMatchCase);
	}

	BOOL operator!=(FindReplaceOption& other) {
		return (this->findString != other.findString)
			|| (this->isMatchWhole != other.isMatchWhole)
			|| (this->isMatchCase != other.isMatchCase);
	}
};

class FindReplaceCommandFactory {
public:
	FindReplaceCommandFactory();
	~FindReplaceCommandFactory();

	Command* Create(CWnd* parent, CFindReplaceDialog* findReplaceDialog);
};


#endif // !_FINDREPLACECOMMANDFACTORY_H
