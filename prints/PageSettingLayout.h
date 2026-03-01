#ifndef _PAGESETTINGLAYOUT_H //guard
#define _PAGESETTINGLAYOUT_H
#include <afxwin.h>

#define CAPTION_SIZE 20

class PageSettingLayout {
public:
	PageSettingLayout(CWnd* parent);
	~PageSettingLayout();

	void Locate();

	RECT& GetPaperArea() const;
	RECT& GetWritingArea() const;
private:
	CWnd* parent;
	RECT paperArea;
	RECT writingArea;
};

inline RECT& PageSettingLayout::GetPaperArea() const {
	return const_cast<RECT&>(this->paperArea);
}

inline RECT& PageSettingLayout::GetWritingArea() const {
	return const_cast<RECT&>(this->writingArea);
}

#endif // !_PAGESETTINGLAYOUT_H

