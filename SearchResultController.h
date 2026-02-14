#ifndef _SEARCHRESULTCONTROLLER_H //guard
#define _SEARCHRESULTCONTROLLER_H
#include <afxwin.h>
#include "findReplaces/FindReplaceOption.h"

typedef signed long int Long;

class SearchResultController {
public:
	SearchResultController(CWnd *parent);
	~SearchResultController();
	SearchResultController(const SearchResultController& source);
	SearchResultController& operator=(const SearchResultController& source);

	Long Search();
	Long Replace(Long index, Long offset);
	Long FindNearestIndexAbove(Long offset);
	Long FindNearestIndexBelow(Long offset);
	void Clear();
	FindReplaceOption& ChangeFindReplaceOption(FindReplaceOption findReplaceOption);
	Long GetAt(Long index);

	Long First();
	Long Previous();
	Long Next();
	Long Move(Long index);

	Long operator[](Long index);

	FindReplaceOption& GetFindReplaceOption() const;
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;
private:
	CWnd* parent;
	FindReplaceOption findReplaceOption;
	Long(*searchResults);
	Long capacity;
	Long length;
	Long current;
};

inline FindReplaceOption& SearchResultController::GetFindReplaceOption() const {
	return const_cast<FindReplaceOption&>(this->findReplaceOption);
}

inline Long SearchResultController::GetCapacity() const {
	return this->capacity;
}

inline Long SearchResultController::GetLength() const {
	return this->length;
}

inline Long SearchResultController::GetCurrent() const {
	return this->current;
}

#endif // !_SEARCHRESULTCONTROLLER_H
