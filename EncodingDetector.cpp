#include "EncodingDetector.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

EncodingDetector::EncodingDetector() {

}

EncodingDetector::~EncodingDetector() {

}

Encoding EncodingDetector::ClassifyEncoding(const char(*str), Long count) {
	Encoding encoding = ANSI;

	if (count > 0)
	{
		if (static_cast<unsigned char>(str[0]) == 0xFF
			&& static_cast<unsigned char>(str[1]) == 0xFE)
		{
			encoding = UTF16LE;
		}
		else if (static_cast<unsigned char>(str[0]) == 0xFE
			&& static_cast<unsigned char>(str[1]) == 0xFF)
		{
			encoding = UTF16BE;
		}
		else if (static_cast<unsigned char>(str[0]) == 0xEF
			&& static_cast<unsigned char>(str[1]) == 0xBB
			&& static_cast<unsigned char>(str[2]) == 0xBF)
		{
			encoding = UTF8BOM;
		}
		else
		{
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

			if (flag == true)
			{
				encoding = UTF8;
			}
		}
	}

	return encoding;
}