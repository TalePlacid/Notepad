#ifndef _PAGELOADER_H //guard
#define _PAGELOADER_H

class CWnd;

class PageLoader {
public:
	PageLoader();
	~PageLoader();

	static void LoadFirst(CWnd* parent);
	static void LoadPrevious(CWnd* parent);
	static void LoadNext(CWnd* parent);
	static void LoadLast(CWnd* parent);
};

#endif // !_PAGELOADER_H

