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
	void AdjustCaretUpToVScroll(Long rowWidth = 0);
	void AdjustCaretDownToVScroll(Long rowWidth = 0);
private:
	CWnd* parent;
};

#endif // !_PAGINGNAVIGATOR_H
