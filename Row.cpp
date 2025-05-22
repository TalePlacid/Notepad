#include "Row.h"
#include "TextOutVisitor.h"

#pragma warning(disable:4996)

Row::Row(Long capacity)
	:Composite(capacity) {
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

void Row::Accept(TextOutVisitor& textOutVisitor) {
	Composite::Accept(textOutVisitor);
	textOutVisitor.VisitRow(this);
}

#if 0

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	Row row;
	Character* character = new SingleByteCharacter('a');
	Long index = row.WriteAtEnd(character);
	cout << dynamic_cast<SingleByteCharacter*>(row.GetAt(index))->GetContent()
		<< endl;

	character = new MultiByteCharacter(const_cast<char*>("°¡"));
	index = row.WriteAtEnd(character);
	cout << dynamic_cast<MultiByteCharacter*>(row.GetAt(index))->GetContent()
		<< endl;

	Row copy(row);
	Long i = 0;
	while (i < copy.GetLength())
	{
		if (dynamic_cast<SingleByteCharacter*>(copy[i]))
		{
			cout << dynamic_cast<SingleByteCharacter*>(copy[i])->GetContent();
		}
		else if (dynamic_cast<MultiByteCharacter*>(copy[i]))
		{
			cout << dynamic_cast<MultiByteCharacter*>(copy[i])->GetContent();
		}
		i++;
	}
	cout <<endl;

	character = new MultiByteCharacter(const_cast<char*>("³ª"));
	row.WriteAtEnd(character);

	copy = row;
	i = 0;
	while (i < copy.GetLength())
	{
		if (dynamic_cast<SingleByteCharacter*>(copy[i]))
		{
			cout << dynamic_cast<SingleByteCharacter*>(copy[i])->GetContent();
		}
		else if (dynamic_cast<MultiByteCharacter*>(copy[i]))
		{
			cout << dynamic_cast<MultiByteCharacter*>(copy[i])->GetContent();
		}
		i++;
	}
	cout << endl;

	return 0;
}

#endif