#ifndef _PREVIEWLAYOUT_H //guard
#define _PREVIEWLAYOUT_H
#include <afxwin.h>

#define CAPTION_SIZE 20

class PreviewLayout {
public:
	PreviewLayout(CWnd* parent);
	~PreviewLayout();

	void Locate();

	RECT& GetPaperArea() const;
	RECT& GetWritingArea() const;
private:
	CWnd* parent;
	RECT paperArea;
	RECT writingArea;
};

inline RECT& PreviewLayout::GetPaperArea() const {
	return const_cast<RECT&>(this->paperArea);
}

inline RECT& PreviewLayout::GetWritingArea() const {
	return const_cast<RECT&>(this->writingArea);
}

#endif // !_PREVIEWLAYOUT_H
