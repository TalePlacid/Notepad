#include "Font.h"

#pragma warning(disable:4996)

Font::Font(CString family, Long size, CString style, CFont* cFont)
	:family(family), style(style) {
	this->size = size;
	this->cFont = cFont;
}

Font::~Font() {
	if (this->cFont != NULL)
	{
		delete this->cFont;
	}
}

Font::Font(const Font& source){
	this->family = source.family;
	this->size = source.size;
	this->style = source.style;
	
	this->cFont = new CFont;
	LOGFONT logFont;
	source.cFont->GetLogFont(&logFont);
	this->cFont->CreateFontIndirectA(&logFont);
}

Font& Font::operator=(const Font& source) {
	this->family = source.family;
	this->size = source.size;
	this->style = source.style;

	if (this->cFont != NULL)
	{
		delete this->cFont;
		this->cFont = NULL;
	}

	this->cFont = new CFont;
	LOGFONT logFont;
	source.cFont->GetLogFont(&logFont);
	this->cFont->CreateFontIndirectA(&logFont);

	return *this;
}