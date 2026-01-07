#ifndef _PAGINGNAVIGATOR_H //guard
#define _PAGINGNAVIGATOR_H

typedef signed long int Long;

class CWnd;

class CaretNavigator {
public:
	CaretNavigator(CWnd* parent);
	~CaretNavigator();

	Long MoveTo(Long offset); 
	void NormalizeColumn(Long columnIndex);
private:
	CWnd* parent;
};

#endif // !_PAGINGNAVIGATOR_H
