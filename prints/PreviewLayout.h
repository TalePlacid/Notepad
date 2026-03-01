#ifndef _PREVIEWLAYOUT_H //guard
#define _PREVIEWLAYOUT_H
#include <afxwin.h>

typedef signed long int Long;

class PreviewLayout {
public:
	PreviewLayout(CWnd* parent);
	~PreviewLayout();

	void Locate();

	RECT& GetClientArea() const;
	RECT& GetPaperArea() const;
	RECT& GetWritingArea() const;
	RECT& GetFirstButtonArea() const;
	RECT& GetPreviousButtonArea() const;
	RECT& GetNextButtonArea() const;
	RECT& GetLastButtonArea() const;
	RECT& GetPageNumberArea() const;
	CPoint& GetHeaderPoint() const;
	CPoint& GetFooterPoint() const;
private:
	CWnd* parent;
	RECT clientArea;
	RECT paperArea;
	RECT writingArea;
	RECT firstButtonArea;
	RECT previousButtonArea;
	RECT nextButtonArea;
	RECT lastButtonArea;
	RECT pageNumberArea;
	CPoint headerPoint;
	CPoint footerPoint;
};

inline RECT& PreviewLayout::GetClientArea() const {
	return const_cast<RECT&>(this->clientArea);
}

inline RECT& PreviewLayout::GetPaperArea() const {
	return const_cast<RECT&>(this->paperArea);
}

inline RECT& PreviewLayout::GetWritingArea() const {
	return const_cast<RECT&>(this->writingArea);
}

inline RECT& PreviewLayout::GetFirstButtonArea() const {
	return const_cast<RECT&>(this->firstButtonArea);
}

inline RECT& PreviewLayout::GetPreviousButtonArea() const {
	return const_cast<RECT&>(this->previousButtonArea);
}

inline RECT& PreviewLayout::GetNextButtonArea() const {
	return const_cast<RECT&>(this->nextButtonArea);
}

inline RECT& PreviewLayout::GetLastButtonArea() const {
	return const_cast<RECT&>(this->lastButtonArea);
}

inline RECT& PreviewLayout::GetPageNumberArea() const {
	return const_cast<RECT&>(this->pageNumberArea);
}

inline CPoint& PreviewLayout::GetHeaderPoint() const {
	return const_cast<CPoint&>(this->headerPoint);
}

inline CPoint& PreviewLayout::GetFooterPoint() const {
	return const_cast<CPoint&>(this->footerPoint);
}

#endif // !_PREVIEWLAYOUT_H

