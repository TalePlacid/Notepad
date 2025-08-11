#include "SearchingAlgorithm.h"

#pragma warning(disable:4996)

SearchingAlgorithm::SearchingAlgorithm(string key, string contents, Comparer* comparer) {
	this->key = key;
	this->contents = contents;
	this->comparer = comparer;
}

SearchingAlgorithm::~SearchingAlgorithm() {

}