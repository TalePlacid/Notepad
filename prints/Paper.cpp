#include "Paper.h"

#pragma warning(disable:4996)

const PaperSize Paper::a4 = { 210, 297 };
const PaperSize Paper::a5 = { 148, 210 };
const PaperSize Paper::a6 = { 105, 148 };
const PaperSize Paper::b4 = { 250, 353 };
const PaperSize Paper::b5 = { 176, 250 };
const PaperSize Paper::letter = { 216, 279 };
const PaperSize Paper::legal = { 216, 356 };

const PaperSize Paper::GetPaperSize(CString paper){
	PaperSize paperSize;
	if (paper == "A4")
	{
		paperSize = Paper::a4;
	}
	else if (paper == "A5")
	{
		paperSize = Paper::a5;
	}
	else if (paper == "A6")
	{
		paperSize = Paper::a6;
	}
	else if (paper == "B4")
	{
		paperSize = Paper::b4;
	}
	else if (paper == "B5")
	{
		paperSize = Paper::b5;
	}
	else if (paper == "레터")
	{
		paperSize = Paper::letter;
	}
	else if (paper == "리걸")
	{
		paperSize = Paper::legal;
	}

	return paperSize;
}

const short Paper::GetDmPaperSize(CString paper) {
	short paperSize;
	if (paper == "A4")
	{
		paperSize = DMPAPER_A4;
	}
	else if (paper == "A5")
	{
		paperSize = DMPAPER_A5;
	}
	else if (paper == "A6")
	{
		paperSize = DMPAPER_A6;
	}
	else if (paper == "B4")
	{
		paperSize = DMPAPER_B4;
	}
	else if (paper == "B5")
	{
		paperSize = DMPAPER_B5;
	}
	else if (paper == "레터")
	{
		paperSize = DMPAPER_LETTER;
	}
	else if (paper == "리걸")
	{
		paperSize = DMPAPER_LEGAL;
	}

	return paperSize;
}
