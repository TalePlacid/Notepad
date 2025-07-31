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

Long Note::FindSelectionStart() {
	Long index = 0;

	Long flag = -1;
	while (index < this->length && flag < 0)
	{
		flag = this->glyphs[index]->FindSelectionStart();
		index++;
	}

	if (index < this->length)
	{
		index--;
	}
	else
	{
		index = -1;
	}

	return index;
}

Long Note::FindSelectionEnd() {
	Long index = this->length - 1;
	Long flag = -1;
	while (index >= 0 && flag < 0)
	{
		flag = this->glyphs[index]->FindSelectionEnd();
		index--;
	}

	if (flag >= 0)
	{
		index++;
	}

	return index;
}

#if 0

#include <iostream>
using namespace std;
#include "Glyph.h"
#include "NoteConverter.h"

int main(int argc, char* argv[]) {
	string contents("나랏말싸미\r\n듕귁에 달아\r\nOne ring\r\nFind them\r\nall.");
	NoteConverter noteConverter;
	Glyph* note = noteConverter.Convert(contents);

	cout << "[선택없음]" << endl
		<< "선택시작줄 : " << note->FindSelectionStart() << endl
		<< "선택시작칸 : " << note->GetAt(0)->FindSelectionStart() << endl
		<< "선택끝줄 : " << note->FindSelectionStart() << endl
		<< "선택끝칸 : " << note->GetAt(0)->FindSelectionEnd() << endl << endl;

	Glyph* row = note->GetAt(1);
	Long i = 2;
	while (i < row->GetLength())
	{
		row->GetAt(i)->Select(true);
		i++;
	}

	cout << "[1줄 선택]" << endl
		<< "선택시작줄 : " << note->FindSelectionStart() << endl
		<< "선택시작칸 : " << note->GetAt(note->FindSelectionStart())->FindSelectionStart() << endl
		<< "선택끝줄 : " << note->FindSelectionEnd() << endl
		<< "선택끝칸 : " << note->GetAt(note->FindSelectionEnd())->FindSelectionEnd() << endl << endl;

	note->GetAt(2)->Select(true);

	row = note->GetAt(3);
	i = 0;
	while (i < 3)
	{
		row->GetAt(i)->Select(true);
		i++;
	}

	cout << "[여러 줄 선택]" << endl
		<< "선택시작줄 : " << note->FindSelectionStart() << endl
		<< "선택시작칸 : " << note->GetAt(note->FindSelectionStart())->FindSelectionStart() << endl
		<< "선택끝줄 : " << note->FindSelectionEnd() << endl
		<< "선택끝칸 : " << note->GetAt(note->FindSelectionEnd())->FindSelectionEnd() << endl << endl;

	return 0;
}

#endif