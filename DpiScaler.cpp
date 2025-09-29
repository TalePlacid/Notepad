#include "DpiScaler.h"

#pragma warning(disable:4996)

DpiScaler::DpiScaler() {

}

DpiScaler::~DpiScaler() {

}

Long DpiScaler::PixelToMm(Long pixel, Long dpi) {
	Long mm = (double)pixel / dpi * 25.4;

	return mm;
}

Long DpiScaler::MmToPixel(Long mm, Long dpi) {
	Long pixel = mm * dpi / 25.4;

	return pixel;
}

