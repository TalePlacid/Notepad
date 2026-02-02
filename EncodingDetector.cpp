#include "EncodingDetector.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

EncodingDetector::EncodingDetector() {

}

EncodingDetector::~EncodingDetector() {

}

bool EncodingDetector::IsUTF16LE(char (*str)){
	bool ret = false;

	if (static_cast<unsigned char>(str[0]) == 0xFF && static_cast<unsigned char>(str[1]) == 0xFE)
	{
		ret = true;
	}

	return ret;
}

bool EncodingDetector::IsUTF16BE(char(*str)) {
	bool ret = false;

	if (static_cast<unsigned char>(str[0]) == 0xFE && static_cast<unsigned char>(str[1]) == 0xFF)
	{
		ret = true;
	}

	return ret;
}

bool EncodingDetector::IsUTF8BOM(char(*str)) {
	bool ret = false;

	if (static_cast<unsigned char>(str[0]) == 0xEF 
		&& static_cast<unsigned char>(str[1]) == 0xBB
		&& static_cast<unsigned char>(str[2]) == 0xBF)
	{
		ret = true;
	}

	return ret;
}

bool EncodingDetector::IsUTF8(char(*str), Long count) {
	ByteChecker byteChecker;
	Long bytes;
	Long j;
	bool flag = true;
	Long i = 0;
	while (i < count && flag)
	{
		flag = false;
		bytes = byteChecker.CheckUtf8FirstByte(str + i);
		if (bytes > 0)
		{
			flag = true;
			j = 1;
			while (j < bytes && flag)
			{
				if (!byteChecker.IsUtf8ContinuationByte(str + i + j))
				{
					flag = false;
				}
				j++;
			}
		}
		i += bytes;
	}

	return flag;
}