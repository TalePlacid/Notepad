#include "SearchResultController.h"

#pragma warning(disable:4996)

SearchResultController::SearchResultController(string key, bool isMatchWhole, bool isMatchCase, bool isSearchDown, Long capacity)
	:key(key){
	this->isMatchWhole = isMatchWhole;
	this->isMatchCase = isMatchCase;
	this->isSearchDown = isSearchDown;
	this->searchResults = new SearchResult[capacity];
	this->capacity = capacity;
	this->length = 0;
	this->current = -1;
}

SearchResultController::SearchResultController(string key, bool isMatchWhole, bool isMatchCase, bool isSearchDown, Long(*offsets), Long count)
	:key(key){
	this->isMatchWhole = isMatchWhole;
	this->isMatchCase = isMatchCase;
	this->isSearchDown = isSearchDown;

	if (count > 0)
	{
		this->searchResults = new SearchResult[count];
	}
	else
	{
		this->searchResults = NULL;
	}
	
	Long i = 0;
	while (i < count)
	{
		this->searchResults[i] = SearchResult(offsets[i]);
		i++;
	}

	this->capacity = count;
	this->length = count;
	this->current = 0;
}

SearchResultController::~SearchResultController() {
	if (this->searchResults != 0)
	{
		delete[] this->searchResults;
	}
}

SearchResultController::SearchResultController(const SearchResultController& source) 
	:key(source.key){
	if (this->searchResults != NULL)
	{
		delete[] this->searchResults;
		this->searchResults = 0;
	}

	this->searchResults = new SearchResult[source.capacity];
	Long i = 0;
	while (i < source.length)
	{
		this->searchResults[i] = source.searchResults[i];
		i++;
	}
	
	this->isMatchWhole = source.isMatchWhole;
	this->isMatchCase = source.isMatchCase;
	this->isSearchDown = source.isSearchDown;
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

SearchResultController& SearchResultController::operator=(const SearchResultController& source) {
	this->key = source.key;

	if (this->searchResults != NULL)
	{
		delete[] this->searchResults;
		this->searchResults = 0;
	}

	this->searchResults = new SearchResult[source.capacity];
	Long i = 0;
	while (i < source.length)
	{
		this->searchResults[i] = source.searchResults[i];
		i++;
	}

	this->isMatchWhole = source.isMatchWhole;
	this->isMatchCase = source.isMatchCase;
	this->isSearchDown = source.isSearchDown;
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}

Long SearchResultController::FindNearestIndexAbove(Long offset) {
	Long index = -1;

	bool flag = false;
	Long i = 0;
	Long searchResultOffset = this->searchResults[i].GetOffset();
	while (i < this->length && searchResultOffset < offset)
	{
		flag = true;
		i++;
		searchResultOffset = this->searchResults[i].GetOffset();
	}

	if (flag = true)
	{
		index = i - 1;
	}

	return index;
}

Long SearchResultController::FindNearestIndexBelow(Long offset) {
	Long index = -1;

	Long i = 0;
	Long searchResultOffset = this->searchResults[i].GetOffset();
	while (i < this->length && searchResultOffset < offset)
	{
		i++;
		searchResultOffset = this->searchResults[i].GetOffset();
	}

	if (i < this->length)
	{
		index = i;
	}

	return index;
}

SearchResult& SearchResultController::GetAt(Long index) {
	return this->searchResults[index];
}

Long SearchResultController::Previous() {
	(this->current)--;
	if (this->current < 0)
	{
		this->current = 0;
	}

	return this->current;
}

Long SearchResultController::Next() {
	(this->current)++;
	if (this->current >= this->length)
	{
		this->current = this->length - 1;
	}

	return this->current;
}

Long SearchResultController::Move(Long index) {
	this->current = index;

	return this->current;
}

SearchResult& SearchResultController::operator[](Long index) {
	return this->searchResults[index];
}