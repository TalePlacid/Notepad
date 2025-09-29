#ifndef _DPISCALER_H //guard
#define _DPISCALER_H

typedef signed long int Long;

class DpiScaler {
public:
	DpiScaler();
	~DpiScaler();

	Long PixelToMm(Long pixel, Long dpi);
	Long MmToPixel(Long mm, Long dpi);
};

#endif // !_DPISCALER_H
