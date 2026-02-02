#ifndef _ENCODINGDETECTOR_H
#define _ENCODINGDETECTOR_H

typedef signed long int Long;

class EncodingDetector {
public:
	EncodingDetector();
	~EncodingDetector();

	bool IsUTF16LE(char(*str));
	bool IsUTF16BE(char(*str));
	bool IsUTF8BOM(char(*str));
	bool IsUTF8(char(*str), Long count);
};

#endif // !_ENCODINGDETECTOR_H
