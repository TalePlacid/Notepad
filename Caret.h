#ifndef _CARET_H //guard
#define _CARET_H
#include <afxwin.h>

typedef signed long int Long;

class NotepadForm;

class Caret {
public:
	Caret(CWnd* parent, Long x, Long y, Long width, Long height);
	~Caret();

	Long GetX() const;
	Long GetY() const;
	Long GetWidth() const;
	Long GetHeight() const;
private:
	CWnd* parent;
	Long x;
	Long y;
	Long width;
	Long height;
};

inline Long Caret::GetX() const {
	return this->x;
}

inline Long Caret::GetY() const {
	return this->y;
}

inline Long Caret::GetWidth() const {
	return this->width;
}

inline Long Caret::GetHeight() const {
	return this->height;
}

#endif // !_CARET_H

