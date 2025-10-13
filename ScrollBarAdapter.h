#ifndef _SCROLLBARADAPTER_H //guard
#define _SCROLLBARADAPTER_H
#include <afxwin.h>

typedef signed long int Long;

class ScrollBarAdapter {
public:
	ScrollBarAdapter(CWnd *parent);
	~ScrollBarAdapter();

	BOOL Create(UINT scrollBar, Long nMin, Long nMax, Long nPage, Long nPos);
	BOOL Destroy(UINT scrollBar);

	Long UpdateMax(UINT scrollBar, Long nMax);
	Long ResizePage(UINT scrollBar, Long nPage);
	Long Move(UINT scrollBar, Long nPos);
	Long Up(Long distance);
	Long Down(Long distance);
	Long Left(Long distance);
	Long Right(Long distance);
	Long PageUp();
	Long PageDown();

	bool HasVScrollBar() const;
	bool HasHScrollBar() const;
	Long GetMax(UINT scrollBar) const;
	Long GetPage(UINT scrollBar) const;
	Long GetPos(UINT scrollBar) const;
private:
	CWnd* parent;
	BOOL hasVScrollBar;
	BOOL hasHScrollBar;
};

inline bool ScrollBarAdapter::HasVScrollBar() const {
	return this->hasVScrollBar;
}

inline bool ScrollBarAdapter::HasHScrollBar() const {
	return this->hasHScrollBar;
}

#endif // !_SCROLLBARADAPTER_H
