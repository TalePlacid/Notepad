#ifndef _BRUTEFORCE_H //guard
#define _BRUTEFORCE_H
#include <string>
using namespace std;
#include "SearchingAlgorithm.h"

class Comparer;

class BruteForce : public SearchingAlgorithm {
public:
	BruteForce(string key, string contents, Comparer* comparer);
	virtual ~BruteForce();

	virtual void DoAlgorithm(Long* (*offsets), Long* count);
};

#endif // !_BRUTEFORCE_H
