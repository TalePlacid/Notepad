#ifndef _ENCODINGDETECTOR_H
#define _ENCODINGDETECTOR_H

typedef signed long int Long;

enum Encoding : Long {
	UTF16LE,
	UTF16BE,
	UTF8BOM,
	UTF8,
	ANSI
};

class EncodingDetector {
public:
	EncodingDetector();
	~EncodingDetector();

	Encoding ClassifyEncoding(char(*str), Long count);
};

#endif // !_ENCODINGDETECTOR_H
