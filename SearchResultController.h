#ifndef _SEARCHRESULTCONTROLLER_H //guard
#define _SEARCHRESULTCONTROLLER_H
#include <string>
using namespace std;
#include "SearchResult.h"

typedef signed long int Long;

class SearchResultController {
public:
	SearchResultController(string key = "", Long capacity = 128);
	SearchResultController(string key, Long(*offsets), Long count);
	~SearchResultController();
	SearchResultController(const SearchResultController& source);
	SearchResultController& operator=(const SearchResultController& source);

	SearchResult& GetAt(Long index);

	Long Previous();
	Long Next();

	SearchResult& operator[](Long index);

	string& GetKey() const;
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;
private:
	SearchResult(*searchResults);
	string key;
	Long capacity;
	Long length;
	Long current;
};

inline string& SearchResultController::GetKey() const {
	return const_cast<string&>(this->key);
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
