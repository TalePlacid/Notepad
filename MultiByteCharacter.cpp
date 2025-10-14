#include "MultiByteCharacter.h"
#include <cstring>

#pragma warning(disable:4996)

MultiByteCharacter::MultiByteCharacter() {
	this->content[0] = '\0';
}

MultiByteCharacter::MultiByteCharacter(char(*content), bool isDirty){
	strncpy(this->content, content, 2);
}

MultiByteCharacter::~MultiByteCharacter() {

}

MultiByteCharacter::MultiByteCharacter(const MultiByteCharacter& source)
	:Character(source) {
	strncpy(this->content, source.content, 2);
}

MultiByteCharacter& MultiByteCharacter::operator=(const MultiByteCharacter& source) {
	Character::operator=(source);
	strncpy(this->content, source.content, 2);

	return *this;
}

MultiByteCharacter& MultiByteCharacter::operator=(const char(*content)) {
	strncpy(this->content, content, 2);

	return *this;
}

Glyph* MultiByteCharacter::Clone() {
	return new MultiByteCharacter(*this);
}

string MultiByteCharacter::MakeString() {
	string str("");
	str.append(this->content, 2);

	return str;
}

bool MultiByteCharacter::IsWordCharacter() {
	return true;
}

bool MultiByteCharacter::IsMultiByteCharacter() {
	return true;
}

bool MultiByteCharacter::IsEqual(const MultiByteCharacter& source) {
	bool ret = false;

	if (strcmp(this->content, source.content) == 0)
	{
		ret = true;
	}

	return ret;
}

bool MultiByteCharacter::IsEqual(const char (*content)) {
	bool ret = false;

	if (strcmp(this->content, content) == 0)
	{
		ret = true;
	}

	return ret;
}

bool MultiByteCharacter::IsNotEqual(const MultiByteCharacter& source) {
	bool ret = false;

	if (strcmp(this->content, source.content) != 0)
	{
		ret = true;
	}

	return ret;
}

bool MultiByteCharacter::IsNotEqual(const char(*content)) {
	bool ret = false;

	if (strcmp(this->content, content) != 0)
	{
		ret = true;
	}

	return ret;
}

bool MultiByteCharacter::operator==(const MultiByteCharacter& source) {
	bool ret = false;

	if (strcmp(this->content, source.content) == 0)
	{
		ret = true;
	}

	return ret;
}

bool MultiByteCharacter::operator==(const char(*content)) {
	bool ret = false;

	if (strcmp(this->content, content) == 0)
	{
		ret = true;
	}

	return ret;
}

bool MultiByteCharacter::operator!=(const MultiByteCharacter& source) {
	bool ret = false;

	if (strcmp(this->content, source.content) != 0)
	{
		ret = true;
	}

	return ret;
}

bool MultiByteCharacter::operator!=(const char(*content)) {
	bool ret = false;

	if (strcmp(this->content, content) != 0)
	{
		ret = true;
	}

	return ret;
}

MultiByteCharacter::operator char*() const {
	return const_cast<char*>(this->content);
}

#if 0

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	MultiByteCharacter character(const_cast<char*>("��"));
	cout << character.GetContent()
		<< endl;

	MultiByteCharacter copy(character);
	cout << copy.GetContent()
		<< endl;

	if (character.IsEqual(copy))
	{
		cout << "���Դϴ�."
			<< endl;
	}

	if (character == copy)
	{
		cout << "���Դϴ�."
			<< endl;
	}

	if (character.IsEqual("��"))
	{
		cout << "���Դϴ�."
			<< endl;
	}

	if (character == "��")
	{
		cout << "���Դϴ�."
			<< endl;
	}
	
	character = "��";
	cout << character.GetContent()
		<< endl;

	if (character.IsNotEqual(copy))
	{
		cout << "���Դϴ�."
			<< endl;
	}

	if (character != copy)
	{
		cout << "���Դϴ�."
			<< endl;
	}

	if (character.IsNotEqual("��"))
	{
		cout << "���Դϴ�."
			<< endl;
	}
	
	if (character != "��")
	{
		cout << "���Դϴ�."
			<< endl;
	}

	copy = character;
	cout << copy.GetContent()
		<< endl;

	return 0;
}

#endif