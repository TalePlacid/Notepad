#include "SearchingAlgorithmFactory.h"
#include "SearchingAlgorithm.h"
#include "Comparer.h"
#include "BruteForce.h"

#pragma warning(disable:4996)

SearchingAlgorithmFactory::SearchingAlgorithmFactory() {

}

SearchingAlgorithmFactory::~SearchingAlgorithmFactory() {

}

SearchingAlgorithm* SearchingAlgorithmFactory::Create(string key, string contents, Comparer* comparer, Algorithm name, bool isWholeMatch) {
	SearchingAlgorithm* searchingAlgorithm = 0;
	switch (name)
	{
	case SearchingAlgorithmFactory::BRUTE_FORCE:
		if (isWholeMatch)
		{

		}
		else
		{
			searchingAlgorithm = new BruteForce(key, contents, comparer);
		}
		break;
	default:
		break;
	}
	

	return searchingAlgorithm;
}