#include "NoteConverter.h"
#include "GlyphFactory.h"
#include "Glyph.h"

#pragma warning(disable:4996)

NoteConverter::NoteConverter() {

}

NoteConverter::~NoteConverter() {

}

Glyph* NoteConverter::Convert(string str) {
	GlyphFactory glyphFactory;
	char character[2];

	character[0] = '\0';
	Glyph* note = glyphFactory.Create(character);

	Long rowIndex;
	Glyph* glyph;
	Glyph* row = 0;
	Long i = 0;
	if (str.length() > 0 && str.at(i) != '\r')
	{
		character[0] = '\r';
		character[1] = '\n';
		row = glyphFactory.Create(character);
		note->Add(row);
	}

	while (i < str.length())
	{
		character[0] = str.at(i);
		if (character[0] == '\r' || character[0] & 0x80)
		{
			i++;
			character[1] = str.at(i);
		}

		glyph = glyphFactory.Create(character);

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