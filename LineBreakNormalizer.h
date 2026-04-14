#ifndef _LINEBREAKNORMALIZER_H //guard
#define _LINEBREAKNORMALIZER_H
#include <afxwin.h>

typedef signed long int Long;

class LineBreakNormalizer {
public:
	LineBreakNormalizer();
	~LineBreakNormalizer();

	static void NormalizeLineBreak(const TCHAR(*source), Long sourceCount,
		TCHAR* (*normalized), Long& normalizedCount);
};

#endif // !_LINEBREAKNORMALIZER_H
