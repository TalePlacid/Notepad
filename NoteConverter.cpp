#include "NoteConverter.h"
#include "GlyphFactory.h"
#include "Glyph.h"

#pragma warning(disable:4996)

NoteConverter::NoteConverter() {

}

NoteConverter::~NoteConverter() {

}

Glyph* NoteConverter::Convert(string str, bool isDirty) {
	GlyphFactory glyphFactory;
	char character[2];

	character[0] = '\0';
	Glyph* note = glyphFactory.Create(character, isDirty);

	character[0] = '\r';
	Glyph* glyph = glyphFactory.Create(character, isDirty);
	Long index = note->Add(glyph);
	Glyph* current = note->GetAt(index);

	Long i = 0;
	while (i < str.length())
	{
		character[0] = str.at(i);
		if (character[0] == '\r' || character[0] & 0x80)
		{
			i++;
			character[1] = str.at(i);
		}

		glyph = glyphFactory.Create(character, isDirty);

		if (character[0] != '\r')
		{
			current->Add(glyph);
		}
		else
		{
			index = note->Add(glyph);
			current = note->GetAt(index);
		}
		i++;
	}

	return note;
}