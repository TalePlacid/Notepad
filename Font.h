#ifndef _FONT_H //guard
#define _FONT_H
#include <afxwin.h>

typedef signed long int Long;

class Font {
public:
	Font(CString family, Long size, CString style, CFont* cFont);
	~Font();
	Font(const Font& source);
	Font& operator=(const Font& source);

	CString& GetFamily() const;
	Long GetSize() const;
	CString& GetStyle() const;
	CFont* GetCFont() const;
private:
	CString family;
	Long size;
	CString style;
	CFont* cFont;
};

inline CString& Font::GetFamily() const {
	return const_cast<CString&>(this->family);
}

inline Long Font::GetSize() const {
	return this->size;
}

inline CString& Font::GetStyle() const {
	return const_cast<CString&>(this->style);
}

inline CFont* Font::GetCFont() const {
	return const_cast<CFont*>(this->cFont);
}

#endif // !_FONT_H
