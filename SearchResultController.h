#ifndef _SEARCHRESULTCONTROLLER_H //guard
#define _SEARCHRESULTCONTROLLER_H
#include <afxwin.h>
#include "SearchResult.h"
#include "FindReplaceOption.h"

typedef signed long int Long;

class SearchResultController {
public:
	SearchResultController(CString findString = "", CString replaceString = "", BOOL isMatchWhole = FALSE, BOOL isMatchCase = FALSE, BOOL isSearchDown = FALSE, Long capacity = 128);
	~SearchResultController();
	SearchResultController(const SearchResultController& source);
	SearchResultController& operator=(const SearchResultController& source);

	Long Replace(Long index, SearchResult searchResult);
	Long FindNearestIndexAbove(Long offset);
	Long FindNearestIndexBelow(Long offset);
	SearchResult& GetAt(Long index);

	Long First();
	Long Previous();
	Long Next();
	Long Move(Long index);

	SearchResult& operator[](Long index);

	FindReplaceOption& GetFindReplaceOption() const;
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;
private:
	FindReplaceOption findReplaceOption;
	SearchResult(*searchResults);
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
