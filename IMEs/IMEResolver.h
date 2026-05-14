#ifndef _IMERESOLVER_H //guard
#define _IMERESOLVER_H
#include <afxwin.h>
#include "ImeID.h"

typedef signed long int Long;

class IMEResolver {
public:
	IMEResolver();
	~IMEResolver();

	static ImeID Resolve(UINT message, WPARAM wParam, LPARAM lParam, 
		BOOL isWaitingForComposition, BOOL isWaitingForConversion, Long compositionCharacterLength);
};

#endif // !_IMERESOLVER_H
