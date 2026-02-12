#ifndef _ENCODINGDETECTOR_H
#define _ENCODINGDETECTOR_H
#include "Encoding.h"

typedef signed long int Long;

class EncodingDetector {
public:
	EncodingDetector();
	~EncodingDetector();

	Encoding ClassifyEncoding(const char(*str), Long count);
};

#endif // !_ENCODINGDETECTOR_H
