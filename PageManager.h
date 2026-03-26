#ifndef _PAGEMANAGER_H //guard
#define _PAGEMANAGER_H

class CWnd;

class PageManager {
public:
	PageManager();
	~PageManager();

	static void LoadFirst(CWnd* parent);
	static void LoadPrevious(CWnd* parent);
	static void LoadNext(CWnd* parent);
	static void LoadLast(CWnd* parent);
	static void TrimIfNeeded(CWnd* parent);
};

#endif // !_PAGEMANAGER_H

