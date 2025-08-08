#ifndef _SEARCHRESULTCONTROLLER_H //guard
#define _SEARCHRESULTCONTROLLER_H
#include "LinkedList.h"
#include "SearchResult.h"

class SearchResultController {
public:
	SearchResultController();
	~SearchResultController();
	SearchResultController(const SearchResultController& source);
	SearchResultController& operator=(const SearchResultController& source);

	SearchResult* TakeIn(SearchResult searchResult);
	SearchResult* TakeOut(SearchResult* searchResult);
	SearchResult* Find(Long offset);
	SearchResult* First();
	SearchResult* Previous();
	SearchResult* Next();
	SearchResult* Last();
	SearchResult* Move(SearchResult* searchResult);

	Long GetLength() const;
	SearchResult* GetCurrent() const;

	static int CompareSearchResultLink(void* one, void* other);
private:
	LinkedList<SearchResult> searchResults;
	Long length;
	SearchResult* current;
};

inline Long SearchResultController::GetLength() const {
	return this->length;
}

inline SearchResult* SearchResultController::GetCurrent() const {
	return const_cast<SearchResult*>(this->current);
}

#endif // !_SEARCHRESULTCONTROLLER_H
