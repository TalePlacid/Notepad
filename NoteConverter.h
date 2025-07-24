#ifndef _NOTECONVERTER_H //guard
#define _NOTECONVERTER_H
#include <string>
using namespace std;

class Glyph;

class NoteConverter {
public:
	NoteConverter();
	virtual ~NoteConverter();

	Glyph* Convert(string str, bool isDirty = true);
};

#endif // !_NOTECONVERTER_H