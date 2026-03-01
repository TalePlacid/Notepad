#include "BruteForceWhole.h"
#include "Comparer.h"
#include "../ByteChecker.h"

#pragma warning(disable:4996)

BruteForceWhole::BruteForceWhole(string key, string contents, Comparer* comparer)
	:SearchingAlgorithm(key, contents, comparer) {

}

BruteForceWhole::~BruteForceWhole() {

}

void BruteForceWhole::DoAlgorithm(Long* (*offsets), Long* count) {
	ByteChecker byteChecker;
	(*offsets) = new Long[this->contents.length()];
	(*count) = 0;

	Long j;
	Long k = 0;

	bool previousIsWord;
	bool currentIsWord = false;
	Long i = 0;
	while (i < this->contents.length())
	{
		previousIsWord = currentIsWord;
		currentIsWord = byteChecker.IsWordCharacter(&this->contents[i]);
		if (!previousIsWord && currentIsWord)
		{
			j = 0;
			while (j < this->key.length() && i < this->contents.length() && 
				comparer->Compare(this->contents[i], this->key[j]) == true)
			{
				previousIsWord = currentIsWord;
				currentIsWord = byteChecker.IsWordCharacter(&this->contents[i]);
				i++;
				j++;
			}

			if (j >= this->key.length())
			{
				if (i < this->contents.length())
				{
					if (!byteChecker.IsWordCharacter(&this->contents[i]))
					{
						(*offsets)[k] = i - key.length();
						k++;
						(*count)++;
					}
				}
				else
				{
					(*offsets)[k] = i - key.length();
					k++;
					(*count)++;
				}
			}
		}

		i++;
	}
}
