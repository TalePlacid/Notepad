#ifndef _BRUTEFORCEWHOLE_H //guard
#define _BRUTEFORCEWHOLE_H
#include <string>
using namespace std;
#include "SearchingAlgorithm.h"

class Comparer;

class BruteForceWhole : public SearchingAlgorithm {
public:
	BruteForceWhole(string key, string contents, Comparer* comparer);
	virtual ~BruteForceWhole();

	virtual void DoAlgorithm(Long* (*offsets), Long* count);
};

#endif // !_BRUTEFORCEWHOLE_H
