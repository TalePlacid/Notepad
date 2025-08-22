#ifndef _SEARCHRESULTCONTROLLER_H //guard
#define _SEARCHRESULTCONTROLLER_H
#include <string>
using namespace std;
#include "SearchResult.h"

typedef signed long int Long;

class SearchResultController {
public:
	SearchResultController(string key = "", bool isMatchWhole = false, bool isMatchCase = false, bool isSearchDown = false, Long capacity = 128);
	SearchResultController(string key, bool isMatchWhole, bool isMatchCase, bool isSearchDown, Long(*offsets), Long count);
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

	string& GetKey() const;
	bool IsMatchWhole() const;
	bool IsMatchCase() const;
	bool IsSearchDown() const;
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;
private:
	SearchResult(*searchResults);
	string key;
	bool isMatchWhole;
	bool isMatchCase;
	bool isSearchDown;
	Long capacity;
	Long length;
	Long current;
};

inline string& SearchResultController::GetKey() const {
	return const_cast<string&>(this->key);
}

inline bool SearchResultController::IsMatchWhole() const {
	return this->isMatchWhole;
}

inline bool SearchResultController::IsMatchCase() const {
	return this->isMatchCase;
}

inline bool SearchResultController::IsSearchDown() const {
	return this->isSearchDown;
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
