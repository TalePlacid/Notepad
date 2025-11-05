#ifndef _PAGINGNAVIGATOR_H //guard
#define _PAGINGNAVIGATOR_H

typedef signed long int Long;

class CWnd;

class PagingNavigator {
public:
	PagingNavigator(CWnd* parent);
	~PagingNavigator();

	Long MoveTo(Long offset);
private:
	CWnd* parent;
};

#endif // !_PAGINGNAVIGATOR_H
