#include "Row.h"
#include "TextOutVisitor.h"

#pragma warning(disable:4996)

Row::Row(Long capacity, bool isDirty)
	:Composite(capacity) {
	this->current = 0;
}

Row::~Row() {
}

Row::Row(const Row& source)
	:Composite(source){

}

Row& Row::operator=(const Row& source) {
	Composite::operator=(source);

	return *this;
}

string Row::MakeString() {
	string str("");

	Long i = 0;
	while (i < this->length)
	{
		str += this->glyphs[i]->MakeString();
		i++;
	}

	return str;
}

Glyph* Row::Clone() {
	return new Row(*this);
}

void Row::Accept(Visitor& visitor) {
	Composite::Accept(visitor);
	visitor.VisitRow(this);
}

Long Row::TruncateAfter(Long index) {
	Long count = 0;
	while (this->length > index)
	{
		if (this->glyphs[index] != NULL)
		{
			delete this->glyphs[index];
		}
		this->glyphs.Delete(index);
		(this->length)--;
		(this->capacity)--;
		count++;
	}

	if (this->current > index)
	{
		this->current = index;
	}

	return count;
}

Long Row::SelectRange(Long start, Long end, bool isSelecting) {
	return Composite::SelectRange(start, end - 1, isSelecting);
}

Long Row::FindPreviousWordStart(Long columnIndex) {
	Long ret = 0;
	bool previous = false;
	bool next = false;
	Long i = columnIndex - 1;
	while (i > 0 && (previous || !next))
	{
		previous = this->glyphs[i - 1]->IsWordCharacter();
		next = this->glyphs[i]->IsWordCharacter();
		i--;
	}

	if (!previous && next)
	{
		ret = i + 1;
	}

	return ret;
}

Long Row::FindNextWordStart(Long columnIndex) {
	Long ret = 0;
	bool previous = false;
	bool next = false;
	Long i = columnIndex + 1;
	while (i < this->length && (previous || !next))
	{
		previous = this->glyphs[i - 1]->IsWordCharacter();
		next = this->glyphs[i]->IsWordCharacter();
		i++;
	}

	if (!previous && next)
	{
		ret = i - 1;
	}
	else if (i >= this->length)
	{
		ret = this->length;
	}

	return ret;
}

#if 0

#include <iostream>
using namespace std;
#include <string>
#include "GlyphFactory.h"
#include "Glyph.h"

int main(int argc, char* argv[]) {
	string str("this is    test");
	GlyphFactory factory;
	Glyph* glyph;
	Row row;

	cout << row.FindPreviousWordStart(0) << endl;
	cout << row.FindNextWordStart(0) << endl << endl;

	char content;
	Long i = 0;
	while (i < str.length())
	{
		content = str.at(i);
		glyph = factory.Create(&content);
		row.Add(glyph);
		i++;
	}
	cout << row.MakeString() << endl << endl;

	cout << row.FindPreviousWordStart(15) << endl;
	cout << row.FindPreviousWordStart(11) << endl;
	cout << row.FindPreviousWordStart(0) << endl << endl;

	cout << row.FindNextWordStart(0) << endl;
	cout << row.FindNextWordStart(3) << endl;
	cout << row.FindNextWordStart(8) << endl;
	cout << row.FindNextWordStart(14) << endl;
	cout << row.FindNextWordStart(15) << endl;
}

#endif