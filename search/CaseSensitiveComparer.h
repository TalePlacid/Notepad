#ifndef _CASESENSITIVECOMPARER_H //guard
#define _CASESENSITIVECOMPARER_H
#include "Comparer.h"

class CaseSensitiveComparer : public Comparer {
public:
	CaseSensitiveComparer();
	virtual ~CaseSensitiveComparer();

	virtual bool Compare(char& one, char& other);
};

#endif // !_CASESENSITIVECOMPARER_H
