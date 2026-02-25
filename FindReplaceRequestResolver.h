#ifndef _FINDREPLACEREQUESTRESOLVER_H //guard
#define _FINDREPLACEREQUESTRESOLVER_H
#include "AppID.h"
#include "FindReplaceOption.h"

class CWnd;

class FindReplaceRequestResolver {
public:
	FindReplaceRequestResolver(CWnd *parent);
	~FindReplaceRequestResolver();

	void SupplementOption(FindReplaceOption& findReplaceOption);
	AppID ResolveAppID(AppID appID, const FindReplaceOption findReplaceOption);
	bool IsCommand(AppID appID);
private:
	CWnd* parent;
};

#endif // !_FINDREPLACEREQUESTRESOLVER_H
