#ifndef _ENCODING_H //guard
#define _ENCODING_H

typedef signed long int Long;

enum Encoding : Long {
	UTF16LE,
	UTF16BE,
	UTF8BOM,
	UTF8,
	ANSI
};

#endif // !_ENCODING_H
