#ifndef _ROWCOUNTER_H //guard
#define _ROWCOUNTER_H
#include <afxwin.h>

typedef signed long int Long;

class RowCounter {
public:
	RowCounter();
	~RowCounter();

	static Long CountRow(CString contents);
};

#endif // !_ROWCOUNTER_H
