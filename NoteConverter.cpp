#include "NoteConverter.h"
#include "glyphs/GlyphFactory.h"
#include "glyphs/Glyph.h"

#pragma warning(disable:4996)

NoteConverter::NoteConverter() {

}

NoteConverter::~NoteConverter() {

}

Glyph* NoteConverter::Convert(string str) {
	char character[2];

	character[0] = '\0';
	Glyph* note = GlyphFactory::Create(character);

	character[0] = '\r';
	character[1] = '\n';
	Glyph* row = GlyphFactory::Create(character);
	Long rowIndex = note->Add(row);

	Glyph* glyph;
	Long i = 0;
	while (i < str.length())
	{
		character[0] = str.at(i);
		if (character[0] == '\r' || character[0] & 0x80)
		{
			i++;
			character[1] = str.at(i);
		}

		glyph = GlyphFactory::Create(character);

		if (character[0] != '\r')
		{
			row->Add(glyph);
		}
		else
		{
			rowIndex = note->Add(glyph);
			row = note->GetAt(rowIndex);
		}
		i++;
	}

	if (note->GetLength() > 0)
	{
		rowIndex = note->Move(0);
		row = note->GetAt(rowIndex);
		row->Move(0);
	}

	return note;
}
