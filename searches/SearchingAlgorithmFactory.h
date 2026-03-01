#ifndef _SEARCHINGALGORITHMFACTORY_H //guard
#define _SEARCHINGALGORITHMFACTORY_H
#include <string>
using namespace std;

class SearchingAlgorithm;
class Comparer;

class SearchingAlgorithmFactory {
public:
	enum Algorithm
	{
		BRUTE_FORCE
	};
public:
	SearchingAlgorithmFactory();
	~SearchingAlgorithmFactory();

	SearchingAlgorithm* Create(string key, string contents, Comparer* comparer, Algorithm name, bool isWholeMatch);
};

#endif // !_SEARCHINGALGORITHMFACTORY_H

