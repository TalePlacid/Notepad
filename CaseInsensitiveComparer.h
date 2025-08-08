#ifndef _CASEINSENSITIVECOMPARER_H //guard
#define _CASEINSENSITIVECOMPARER_H
#include "Comparer.h"

class CaseInsensitiveComparer : public Comparer {
public:
	CaseInsensitiveComparer();
	virtual ~CaseInsensitiveComparer();

	virtual bool Compare(char& one, char& other);
};

#endif // !_CASEINSENSITIVECOMPARER_H
