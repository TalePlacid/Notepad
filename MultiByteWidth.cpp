#include "MultiByteWidth.h"

#pragma warning(disable:4996)

MultiByteWidth::MultiByteWidth() {
	this->character[0] = '\0';
	this->character[1] = '\0';
	this->width = 0;
}

MultiByteWidth::MultiByteWidth(const char(*character), Long width) {
	this->character[0] = character[0];
	this->character[1] = character[1];
	this->width = width;
}

MultiByteWidth::~MultiByteWidth() {

}

MultiByteWidth::MultiByteWidth(const MultiByteWidth& source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->width = source.width;
}

MultiByteWidth& MultiByteWidth::operator=(const MultiByteWidth& source) {
	this->character[0] = source.character[0];
	this->character[1] = source.character[1];
	this->width = source.width;

	return *this;
}

bool MultiByteWidth::IsEqual(const MultiByteWidth& other) {
	bool isEqual = false;

	if (this->character[0] == other.character[0]
		&& this->character[1] == other.character[1]
		&& this->width == other.width)
	{
		isEqual = true;
	}

	return isEqual;
}

bool MultiByteWidth::IsNotEqual(const MultiByteWidth& other) {
	bool isNotEqual = false;

	if (this->character[0] != other.character[0]
		|| this->character[1] != other.character[1]
		|| this->width != other.width)
	{
		isNotEqual = true;
	}

	return isNotEqual;
}

bool MultiByteWidth::operator==(const MultiByteWidth& other) {
	bool isEqual = false;

	if (this->character[0] == other.character[0]
		&& this->character[1] == other.character[1]
		&& this->width == other.width)
	{
		isEqual = true;
	}

	return isEqual;
}

bool MultiByteWidth::operator!=(const MultiByteWidth& other) {
	bool isNotEqual = false;

	if (this->character[0] != other.character[0]
		|| this->character[1] != other.character[1]
		|| this->width != other.width)
	{
		isNotEqual = true;
	}

	return isNotEqual;
}
