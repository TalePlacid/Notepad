#include "SearchResultController.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"
#include "searches/Comparer.h"
#include "searches/CaseSensitiveComparer.h"
#include "searches/SearchingAlgorithm.h"
#include "searches/CaseInsensitiveComparer.h"
#include "searches/SearchingAlgorithmFactory.h"

#pragma warning(disable:4996)

SearchResultController::SearchResultController(CWnd *parent){
	this->parent = parent;
	this->searchResults = NULL;
	this->capacity = 0;
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

	this->searchResults = new Long[source.capacity];
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

	this->searchResults = new Long[source.capacity];
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

Long SearchResultController::Search() {
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	CString contents = pagingBuffer->GetFullText();
	
	Comparer* comparer;
	if (this->findReplaceOption.isMatchCase)
	{
		comparer = new CaseSensitiveComparer;
	}
	else
	{
		comparer = new CaseInsensitiveComparer;
	}

	Long(*offsets) = NULL;
	Long count = 0;
	SearchingAlgorithmFactory searchingAlgorithmFactory;
	SearchingAlgorithm* searchingAlgorithm = searchingAlgorithmFactory.Create((LPCTSTR)(this->findReplaceOption.findString),
		(LPCTSTR)contents, comparer, SearchingAlgorithmFactory::BRUTE_FORCE, this->findReplaceOption.isMatchWhole);
	
	if (searchingAlgorithm != NULL)
	{
		searchingAlgorithm->DoAlgorithm(&offsets, &count);
		delete searchingAlgorithm;
	}

	if (comparer != NULL)
	{
		delete comparer;
	}

	if (count > 0)
	{
		this->searchResults = offsets;
		this->capacity = count;
		this->length = count;
	}
	else
	{
		if (offsets != NULL)
		{
			delete[] offsets;
		}
	}

	return this->length;
}

Long SearchResultController::Replace(Long index, Long offset) {
	this->searchResults[index] = offset;

	return index;
}

Long SearchResultController::FindNearestIndexAbove(Long offset) {
	Long index = -1;

	bool flag = false;
	Long i = 0;
	while (i < this->length && this->searchResults[i] < offset)
	{
		flag = true;
		i++;
	}

	if (flag && this->searchResults[i] >= offset)
	{
		index = i - 1;
	}

	return index;
}

Long SearchResultController::FindNearestIndexBelow(Long offset) {
	Long index = -1;

	Long i = 0;
	while (i < this->length && this->searchResults[i] < offset)
	{
		i++;
	}

	if (i < this->length)
	{
		index = i;
	}

	return index;
}

void SearchResultController::Clear() {
	if (this->searchResults != NULL)
	{
		delete[] this->searchResults;
		this->searchResults = NULL;
	}
	this->capacity = 0;
	this->length = 0;
	this->current = -1;
}

FindReplaceOption& SearchResultController::ChangeFindReplaceOption(FindReplaceOption findReplaceOption) {
	this->findReplaceOption = findReplaceOption;

	return this->findReplaceOption;
}

Long SearchResultController::GetAt(Long index) {
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

Long SearchResultController::operator[](Long index) {
	return this->searchResults[index];
}
