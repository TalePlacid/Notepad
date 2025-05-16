#include "SingleByteCharacter.h"

#pragma warning(disable:4996)

SingleByteCharacter::SingleByteCharacter(char content) {
	this->content = content;
}

SingleByteCharacter::~SingleByteCharacter() {

}

SingleByteCharacter::SingleByteCharacter(const SingleByteCharacter& source)
	:Character(source) {
	this->content = source.content;
}

SingleByteCharacter& SingleByteCharacter::operator=(const SingleByteCharacter& source) {
	Character::operator=(source);

	this->content = source.content;

	return *this;
}

SingleByteCharacter& SingleByteCharacter::operator=(const char content) {
	this->content = content;

	return *this;
}

Glyph* SingleByteCharacter::Clone() {
	return new SingleByteCharacter(*this);
}

string SingleByteCharacter::MakeString() {
	string str("");
	if (this->content != '\t')
	{
		str.append(1, this->content);
	}
	else
	{
		str.append(8, ' ');
	}

	return str;
}

bool SingleByteCharacter::IsEqual(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content == other.content)
	{
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::IsEqual(const char content) {
	bool ret = false;

	if (this->content == content)
	{
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::IsNotEqual(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content != other.content)
	{
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::IsNotEqual(const char content) {
	bool ret = false;

	if (this->content != content)
	{
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator==(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content == other.content)
	{
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator==(const char content) {
	bool ret = false;

	if (this->content == content)
	{
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator!=(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content != other.content)
	{
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator!=(const char content) {
	bool ret = false;

	if (this->content != content)
	{
		ret = true;
	}

	return ret;
}

#if 0

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	SingleByteCharacter character('a');
	cout << character.GetContent()
		<< endl;
	
	SingleByteCharacter copy(character);
	cout << copy.GetContent()
		<< endl;

	if (character.IsEqual(copy))
	{
		cout << "참입니다."
			<< endl;
	}

	if (character == copy)
	{
		cout << "참입니다."
			<< endl;
	}
	
	if (character.IsEqual('a'))
	{
		cout << "참입니다."
			<< endl;
	}

	if (character == 'a')
	{
		cout << "참입니다."
			<< endl;
	}

	character = 'b';
	cout << character.GetContent()
		<< endl;

	if (character.IsNotEqual(copy))
	{
		cout << "참입니다."
			<< endl;
	}

	if (character != copy)
	{
		cout << "참입니다."
			<< endl;
	}

	if (character.IsNotEqual('a'))
	{
		cout << "참입니다."
			<< endl;
	}

	if (character != 'a')
	{
		cout << "참입니다."
			<< endl;
	}

	copy = character;
	cout << copy.GetContent()
		<< endl;

	return 0;
}

#endif