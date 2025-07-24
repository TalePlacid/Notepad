#include "GlyphFactory.h"
#include "Note.h"
#include "Row.h"
#include "DummyRow.h"
#include "SingleByteCharacter.h"
#include "MultiByteCharacter.h"

#pragma warning(disable:4996)

GlyphFactory::GlyphFactory() {

}

GlyphFactory::~GlyphFactory(){

}

Glyph* GlyphFactory::Create(char(*content), bool isDirty) {
	Glyph* glyph = 0;

	if (content[0] == '\0')
	{
		glyph = new Note;
	}
	else if (content[0] == '\r')
	{
		glyph = new Row(isDirty);
	}
	else if (content[0] == '\n')
	{
		glyph = new DummyRow(isDirty);
	}
	else if (content[0] & 0x80)
	{
		glyph = new MultiByteCharacter(content, isDirty);
	}
	else
	{
		glyph = new SingleByteCharacter(content[0], isDirty);
	}


	return glyph;
}