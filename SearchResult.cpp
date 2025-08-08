#include "SearchResult.h"

#pragma warning(disable:4996)

SearchResult::SearchResult(Long offset) {
	this->offset = offset;
}

SearchResult::~SearchResult() {

}

SearchResult::SearchResult(const SearchResult& source) {
	this->offset = source.offset;
}

SearchResult& SearchResult::operator=(const SearchResult& source) {
	this->offset = source.offset;

	return *this;
}