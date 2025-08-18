#include "BruteForce.h"
#include "Comparer.h"

#pragma warning(disable:4996)

BruteForce::BruteForce(string key, string contents, Comparer* comparer)
	:SearchingAlgorithm(key, contents, comparer){

}

BruteForce::~BruteForce() {

}

void BruteForce::DoAlgorithm(Long* (*offsets), Long* count) {
	(*offsets) = new Long[this->contents.length()];
	(*count) = 0;

	Long k = 0;
	Long j = 0;
	Long i = 0;
	while (i < this->contents.length())
	{
		if (this->comparer->Compare(this->contents[i], this->key[j]) == true)
		{
			j++;
		}
		else
		{
			j = 0;
		}

		if (j == this->key.length())
		{
			(*offsets)[k] = i - (this->key.length() - 1);
			k++;
			(*count)++;
			j = 0;
		}

		i++;
	}
}