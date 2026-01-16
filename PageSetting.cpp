#include "PageSetting.h"

#pragma warning(disable:4996)

PageSetting::PageSetting() {
	this->paperSize.width = 0;
	this->paperSize.height = 0;
}

PageSetting::PageSetting(Long width, Long height, RECT margin, CString header, CString footer)
	:margin(margin), header(header), footer(footer) {
	this->paperSize.width = width;
	this->paperSize.height = height;
}

PageSetting::PageSetting(const PaperSize paperSize, RECT margin, CString header, CString footer)
	:margin(margin), header(header), footer(footer) {
	this->paperSize = paperSize;
}

PageSetting::~PageSetting() {

}