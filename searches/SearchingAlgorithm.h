#ifndef _SEARCHINGALGORITHM_H //guard
#define _SEARCHINGALGORITHM_H
#include <string>
using namespace std;

typedef signed long int Long;

class Comparer;

class SearchingAlgorithm {
public:
	SearchingAlgorithm(string key, string contents, Comparer* comparer);
	virtual ~SearchingAlgorithm() = 0;

	virtual void DoAlgorithm(Long*(*offset), Long *count) = 0;
protected:
	string key;
	string contents;
	Comparer* comparer;
};

#endif // !_SEARCHINGALGORITHM_H
