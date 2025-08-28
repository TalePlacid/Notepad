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

Long Note::SplitRows(Long rowIndex, Long columnIndex) {
	GlyphFactory glyphFactory;
	char contents[2];
	contents[0] = '\r';
	contents[1] = '\n';
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

#if 0

#include <iostream>
using namespace std;
#include "Glyph.h"
#include "NoteConverter.h"

int main(int argc, char* argv[]) {
	string contents("�������ι�\r\n��ο� �޾�\r\nOne ring\r\nFind them\r\nall.");
	NoteConverter noteConverter;
	Glyph* note = noteConverter.Convert(contents);

	cout << "[���þ���]" << endl
		<< "���ý����� : " << note->FindSelectionStart() << endl
		<< "���ý���ĭ : " << note->GetAt(0)->FindSelectionStart() << endl
		<< "���ó��� : " << note->FindSelectionStart() << endl
		<< "���ó�ĭ : " << note->GetAt(0)->FindSelectionEnd() << endl << endl;

	Glyph* row = note->GetAt(1);
	Long i = 2;
	while (i < row->GetLength())
	{
		row->GetAt(i)->Select(true);
		i++;
	}

	cout << "[1�� ����]" << endl
		<< "���ý����� : " << note->FindSelectionStart() << endl
		<< "���ý���ĭ : " << note->GetAt(note->FindSelectionStart())->FindSelectionStart() << endl
		<< "���ó��� : " << note->FindSelectionEnd() << endl
		<< "���ó�ĭ : " << note->GetAt(note->FindSelectionEnd())->FindSelectionEnd() << endl << endl;

	note->GetAt(2)->Select(true);

	row = note->GetAt(3);
	i = 0;
	while (i < 3)
	{
		row->GetAt(i)->Select(true);
		i++;
	}

	cout << "[���� �� ����]" << endl
		<< "���ý����� : " << note->FindSelectionStart() << endl
		<< "���ý���ĭ : " << note->GetAt(note->FindSelectionStart())->FindSelectionStart() << endl
		<< "���ó��� : " << note->FindSelectionEnd() << endl
		<< "���ó�ĭ : " << note->GetAt(note->FindSelectionEnd())->FindSelectionEnd() << endl << endl;

	row = note->GetAt(2);
	Long count = row->TruncateAfter(1);
	cout << "[ĭ ���� �߶󳻱�]" << endl
		<< row->MakeString() << endl
		<< count << "ĭ �߷Ƚ��ϴ�." << endl << endl;


	count = note->TruncateAfter(2);
	cout << "[�� ���� �߶󳻱�]" << endl
		<< note->MakeString() << endl
		<< count << "�� �߷Ƚ��ϴ�." << endl << endl;

	row = note->GetAt(0);
	count = row->TruncateBefore(3);
	cout << "[ĭ ���� �߶󳻱�]" << endl
		<< row->MakeString() << endl
		<< count << "ĭ �߷Ƚ��ϴ�." << endl << endl;

	count = note->TruncateBefore(2);
	cout << "[�� ���� �߶󳻱�]" << endl
		<< note->MakeString() << endl
		<< count << "�� �߷Ƚ��ϴ�." << endl << endl;

	Glyph* refilled = noteConverter.Convert(contents);
	refilled->MergeRows(0);
	cout << refilled->MakeString() << endl << endl;

	refilled->MergeRows(1);
	refilled->MergeRows(1);
	cout << refilled->MakeString() << endl << endl;

	return 0;
}

#endif