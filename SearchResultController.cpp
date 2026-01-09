#include "SearchResultController.h"

#pragma warning(disable:4996)

SearchResultController::SearchResultController(CString findString, CString replaceString, BOOL isMatchWhole, BOOL isMatchCase, BOOL isSearchDown, Long capacity)
	:findReplaceOption(findString, replaceString, isMatchWhole, isMatchCase, isSearchDown){
	this->searchResults = new SearchResult[capacity];
	this->capacity = capacity;
	this->length = 0;
	this->current = -1;
}

SearchResultController::~SearchResultController() {
	if (this->searchResults != 0)
	{
		delete[] this->searchResults;
	}
}

SearchResultController::SearchResultController(const SearchResultController& source) 
	:findReplaceOption(source.findReplaceOption){
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
	
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

SearchResultController& SearchResultController::operator=(const SearchResultController& source) {
	this->findReplaceOption = source.findReplaceOption;

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

	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}

Long SearchResultController::Replace(Long index, SearchResult searchResult) {
	this->searchResults[index] = searchResult;

	return index;
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

Long SearchResultController::First() {
	this->current = 0;

	return this->current;
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