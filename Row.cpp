#include "Row.h"
#include "TextOutVisitor.h"

#pragma warning(disable:4996)

Row::Row(Long capacity, bool isDirty)
	:Composite(capacity, isDirty) {
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

Long Row::FindSelectionStart() {
	bool isSelected = true;
	Long index = this->glyphs.LinearSearchUnique(&isSelected, this->CompareSelection);

	return index;
}

Long Row::FindSelectionEnd() {
	Long index = this->length - 1;
	bool isSelected = false;
	while (index >= 0 && !isSelected)
	{
		isSelected = this->glyphs[index]->IsSelected();
		index--;
	}

	if (isSelected == true)
	{
		index += 2;
	}
	
	return index;
}

int Row::CompareSelection(void* one, void* other) {
	Glyph** one_ = static_cast<Glyph**>(one);
	bool* other_ = static_cast<bool*>(other);

	return (*one_)->IsSelected() == *other_;
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