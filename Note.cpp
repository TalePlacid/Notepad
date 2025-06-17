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

	for (Long i = 1; i <= 2; i++)
	{
		str.pop_back();
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

void Note::Update(Subject* subject, string interest) {
	if (interest == "Unselect")
	{
		this->Select(false);
	}
}

#if 0

#include <iostream>
using namespace std;
#include "Row.h"
#include "SingleByteCharacter.h"
#include "MultiByteCharacter.h"

int main(int argc, char* argv[]) {
	/*
		Note note(1);
		Long index = note.Add(new Row(2));
		Long subscript = note.GetAt(index)->Add(new SingleByteCharacter('['));
		cout << note[index]->GetAt(subscript)->MakeString()
			<< endl;

		note[index]->Add(new SingleByteCharacter('1'));
		cout << note[index]->MakeString()
			<< endl;

		note[index]->Add(new SingleByteCharacter(']'));
		cout << note[index]->MakeString()
			<< endl;

		index = note.Add(new Row(2));
		note[index]->Add(new SingleByteCharacter('\t'));
		note[index]->Add(new MultiByteCharacter(const_cast<char*>("동")));
		cout << note.MakeString()
			<< endl;

		subscript = note[index]->Add(new MultiByteCharacter(const_cast<char*>("해")));
		cout << note[index]->GetAt(subscript)->MakeString()
			<< endl;

		note[index]->Add(new MultiByteCharacter(const_cast<char*>("물")));
		cout << note[index]->MakeString()
			<< endl << endl;

		Note copy(note);
		cout << copy.MakeString()
			<< endl << endl;

		note[index]->Add(new MultiByteCharacter(const_cast<char*>("과")));
		copy = note;
		cout << copy.MakeString()
			<< endl;
	*/
	Note note("동해물과 백두산이\r\nLet me go\r\n<<virtual>>\r\n\t탭문자 테스트");
	cout << note.MakeString() << ", "
		<< endl <<endl;

	GlyphFactory glyphFactory;
	char content = ' ';
	Glyph* glyph = glyphFactory.Create(&content);

	note.First();
	Long rowIndex = note.Next();
	Glyph* row = note[rowIndex];
	row->First();

	Long i;
	for (i = 1; i <= 4; i++)
	{
		row->Next();
	}

	Long columnIndex = row->GetCurrent();
	columnIndex = row->Add(columnIndex + 1, glyph);
	Glyph* character = row->GetAt(columnIndex);
	cout << note.MakeString()
		<< endl;

	glyph = glyphFactory.Create(const_cast<char*>("남"));
	columnIndex = row->Add(columnIndex + 1, glyph);
	character = row->GetAt(columnIndex);
	cout << character->MakeString()
		<< endl;

	glyph = glyphFactory.Create(const_cast<char*>("산"));
	columnIndex = row->Add(columnIndex + 1, glyph);
	character = row->GetAt(columnIndex);
	cout << character->MakeString()
		<< endl;

	columnIndex = row->First();
	content = '1';
	glyph = glyphFactory.Create(&content);
	columnIndex = row->Add(columnIndex + 1, glyph);
	character = row->GetAt(columnIndex);
	cout << character->MakeString()
		<< endl << endl;

	content = '\r';
	glyph = glyphFactory.Create(&content);
	rowIndex = note.GetCurrent();
	rowIndex = note.Add(rowIndex + 1, glyph);
	
	Glyph* nextRow = note[rowIndex];
	i = columnIndex + 1;
	while (i < row->GetLength())
	{
		character = row->GetAt(i);
		nextRow->Add(character->Clone());
		row->Remove(i);
	}

	columnIndex = nextRow->First();
	cout << note.MakeString()
		<< endl << endl;

	content = '\r';
	glyph = glyphFactory.Create(&content);
	rowIndex = note.GetCurrent();
	rowIndex = note.Add(rowIndex + 1, glyph);
	Glyph* nextNextRow = note[rowIndex];

	i = columnIndex + 1;
	while (i < nextRow->GetLength())
	{
		character = nextRow->GetAt(i);
		nextNextRow->Add(character->Clone());
		nextRow->Remove(i);
	}

	columnIndex = nextNextRow->First();

	cout << note.MakeString()
		<< endl << endl;

	return 0;
}

#endif