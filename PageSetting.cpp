#include "PageSetting.h"

#pragma warning(disable:4996)

PageSetting::PageSetting() {
	this->isVertical = FALSE;
	this->margin.left = 0;
	this->margin.right = 0;
	this->margin.top = 0;
	this->margin.bottom = 0;
}

PageSetting::PageSetting(CString paperName, BOOL isVertical, Margin margin, CString header, CString footer)
	: paperName(paperName), margin(margin), header(header), footer(footer) {
	this->isVertical = isVertical;
}

PageSetting::~PageSetting() {

}