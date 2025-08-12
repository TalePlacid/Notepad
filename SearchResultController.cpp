#include "SearchResultController.h"

#pragma warning(disable:4996)

SearchResultController::SearchResultController(string key, Long capacity)
	:key(key){
	this->searchResults = new SearchResult[capacity];
	this->capacity = capacity;
	this->length = 0;
	this->current = -1;
}

SearchResultController::SearchResultController(string key, Long(*offsets), Long count)
	:key(key){
	this->searchResults = new SearchResult[count];
	
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

	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
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

SearchResult& SearchResultController::operator[](Long index) {
	return this->searchResults[index];
}