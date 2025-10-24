#include "Note.h"
#include "GlyphFactory.h"
#include "DummyRow.h"

#pragma warning(disable:4996)

Note::Note(Long capacity)
	: Composite(capacity){
}

Note::Note(string str, Long capapcity)
	:Composite(capacity) {
	GlyphFactory glyphFactory;
	char character[2];
	
	character[0] = '\r';
	Glyph* glyph = glyphFactory.Create(character);
	
	Long index = this->Add(glyph);
	Glyph* current = this->GetAt(index);

	Long i = 0;
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
			current->Add(glyph);
		}
		else
		{
			index = this->Add(glyph);
			current = this->GetAt(index);
		}
		i++;
	}

	this->First();
	Glyph *row = this->GetAt(this->current);
	row->First();
}

Note::~Note() {

}

Note::Note(const Note& source)
	:Composite(source) {

}

Note& Note::operator=(const Note& source) {
	Composite::operator=(source);

	return *this;
}

Glyph* Note::Clone() {
	return new Note(*this);
}

string Note::MakeString() {
	string str("");

	Long i = 0;
	while (i < this->length)
	{
		str += this->glyphs[i]->MakeString() + string("\r\n");
		i++;
	}

	if (this->length > 0)
	{
		for (Long i = 1; i <= 2; i++)
		{
			str.pop_back();
		}
	}

	return str;
}

Long Note::Next() {
	(this->current)++;
	if (this->current > this->length - 1)
	{
		this->current = this->length - 1;
	}

	return this->current;
}

Long Note::Last() {
	this->current = this->length - 1;

	return this->current;
}

Long Note::MergeRows(Long index) {
	Glyph* row = this->glyphs[index];
	Glyph* nextRow = this->glyphs[index + 1];

	Long i = 0;
	while (i < nextRow->GetLength())
	{
		row->Add(nextRow->GetAt(i)->Clone());
		i++;
	}

	if (this->glyphs[index+1] != NULL)
	{
		delete this->glyphs[index + 1];
	}

	this->glyphs.Delete(index + 1);
	(this->length)--;
	(this->capacity)--;

	return index;
}

Long Note::SplitRows(Long rowIndex, Long columnIndex, bool isDummyRow) {
	GlyphFactory glyphFactory;
	char contents[2];
	if (!isDummyRow)
	{
		contents[0] = '\r';
		contents[1] = '\n';
	}
	else
	{
		contents[0] = '\n';
	}
	Glyph* newRow = glyphFactory.Create(contents);

	Long index = -1;
	if (rowIndex < this->length)
	{
		index = this->glyphs.Insert(rowIndex + 1, newRow);
		(this->capacity)++;
	}
	else if (rowIndex < this->capacity)
	{
		index = this->glyphs.Store(this->length, newRow);
	}
	else
	{
		index = this->glyphs.AppendFromRear(newRow);
		(this->capacity)++;
	}
	(this->length)++;

	Glyph* row = this->glyphs[rowIndex];
	Long i = columnIndex;
	while (i < row->GetLength())
	{
		newRow->Add(row->GetAt(i)->Clone());
		i++;
	}

	row->TruncateAfter(columnIndex);

	return index;
}

void Note::SelectRange(Position start, Position end) {
	Glyph* row;
	Long i;
	if (start.GetRow() < end.GetRow())
	{
		row = this->glyphs[start.GetRow()];
		i = start.GetColumn();
		while (i < row->GetLength())
		{
			row->GetAt(i)->Select(true);
			i++;
		}

		i = start.GetRow() + 1;
		while (i < end.GetRow())
		{
			this->glyphs[i]->Select(true);
			i++;
		}

		row = this->glyphs[end.GetRow()];
		i = 0;
		while (i < row->GetLength())
		{
			row->GetAt(i)->Select(true);
			i++;
		}
	}
	else if (start.GetRow() == end.GetRow())
	{
		row = this->glyphs[start.GetRow()];
		i = start.GetColumn();
		while (i < end.GetColumn())
		{
			row->GetAt(i)->Select(true);
			i++;
		}
	}
}

void Note::Append(const Glyph* other) {
	Long i = 0;
	while (i < other->GetLength())
	{
		if (this->length < this->capacity)
		{
			this->glyphs.Store(this->length, const_cast<Glyph*>(other)->GetAt(i)->Clone());
		}
		else
		{
			this->glyphs.AppendFromRear(const_cast<Glyph*>(other)->GetAt(i)->Clone());
			(this->capacity)++;
		}
		(this->length++);
		i++;
	}
}

bool Note::IsAboveBottomLine() {
	Long bottomLine = this->length / 10 * 7;

	return this->current < bottomLine;
}