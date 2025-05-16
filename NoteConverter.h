#ifndef _NOTECONVERTER_H //guard
#define _NOTECONVERTER_H
#include <string>
using namespace std;

class Glyph;

class NoteConverter {
public:
	NoteConverter();
	virtual ~NoteConverter();

	Glyph* ConvertToNote(string str);
};

#endif // !_NOTECONVERTER_H