#ifndef _PAPER_H //guard
#define _PAPER_H
#include <afxwin.h>

typedef signed long int Long;

struct PaperSize {
	Long width;
	Long height;
};

struct Paper {
	static const PaperSize a4;
	static const PaperSize a5;
	static const PaperSize a6;
	static const PaperSize b4;
	static const PaperSize b5;
	static const PaperSize letter;
	static const PaperSize legal;

	static const PaperSize GetPaperSize(CString paper);
	static const short GetDmPaperSize(CString paper);
};

#endif // !_PAPER_H
