#include "SearchResultController.h"

#pragma warning(disable:4996)

SearchResultController::SearchResultController() {
	this->length = 0;
	this->current = 0;
}

SearchResultController::~SearchResultController() {

}

SearchResultController::SearchResultController(const SearchResultController& source)
	:searchResults(source.searchResults) {
	this->length = source.length;
	this->current = &(this->searchResults.GetCurrent()->GetObject());
}

SearchResultController& SearchResultController::operator=(const SearchResultController& source) {
	this->searchResults = source.searchResults;
	this->length = source.length;
	this->current = &(this->searchResults.GetCurrent()->GetObject());

	return *this;
}

SearchResult* SearchResultController::TakeIn(SearchResult searchResult) {
	LinkedList<SearchResult>::Node *node = this->searchResults.AppendFromTail(searchResult);
	(this->length)++;
	this->current = &(node->GetObject());

	return this->current;
}

SearchResult* SearchResultController::TakeOut(SearchResult* searchResult) {
	LinkedList<SearchResult>::Node* node = this->searchResults.LinearSearchUnique(searchResult, this->CompareSearchResultLink);
	node = this->searchResults.Delete(node);
	
	SearchResult* ret;
	if (node == 0)
	{
		ret = 0;
	}
	else
	{
		ret = &(node->GetObject());
	}

	return ret;
}

SearchResult* SearchResultController::Find(Long offset) {
	LinkedList<SearchResult>::Node *node = this->searchResults.LinearSearchUnique(&offset, )
}

static int CompareSearchResult(void* one, void* other) {
	SearchResult* one_ = static_cast<SearchResult*>(one);
	SearchResult* other_ = static_cast<SearchResult*>(other);

	return one_ == other_;
}