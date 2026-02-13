#ifndef _COMPARER_H //guard
#define _COMPARER_H

class Comparer {
public:
	Comparer();
	virtual ~Comparer() = 0;

	virtual bool Compare(char& one, char& other) = 0;
};

#endif // !_COMPARER_H
