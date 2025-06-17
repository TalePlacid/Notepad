#include "Position.h"

#pragma warning(disable:4996)

Position::Position(Long row, Long column) {
	this->row = row;
	this->column = column;
}

Position::~Position() {

}

Position::Position(const Position& source) {
	this->row = source.row;
	this->column = source.column;
}

Position& Position::operator=(const Position& source) {
	this->row = source.row;
	this->column = source.column;

	return *this;
}

Position Position::Right(Long distance) {
	return Position(this->row, this->column + distance);
}

Position Position::Left(Long distance) {
	Long column = this->column - distance;
	if (column < 0)
	{
		column = 0;
	}

	return Position(this->row, column);
}

Position Position::Up(Long disatance) {
	Long row = this->row - disatance;
	if (row < 0)
	{
		row = 0;
	}

	return Position(row, this->column);
}

Position Position::Down(Long distance) {
	return Position(this->row + distance, this->column);
}

Position Position::Move(Long row, Long column) {
	return Position(row, column);
}

bool Position::IsEqual(const Position& other) {
	bool ret = false;

	if ((this->row == other.row) && (this->column == other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::IsNotEqual(const Position& other) {
	bool ret = false;

	if ((this->row != other.row) || (this->column != other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::IsLesserThan(const Position& other) {
	bool ret = false;

	if (this->row < other.row)
	{
		ret = true;
	}
	else if ((this->row == other.row) && (this->column < other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::IsGreaterThan(const Position& other) {
	bool ret = false;

	if (this->row > other.row)
	{
		ret = true;
	}
	else if ((this->row == other.row) && (this->column > other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::operator==(const Position& other) {
	bool ret = false;

	if ((this->row == other.row) && (this->column == other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::operator!=(const Position& other) {
	bool ret = false;

	if ((this->row != other.row) || (this->column != other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::operator<(const Position& other) {
	bool ret = false;

	if (this->row < other.row)
	{
		ret = true;
	}
	else if ((this->row == other.row) && (this->column < other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::operator<=(const Position& other) {
	bool ret = false;

	if (this->row < other.row)
	{
		ret = true;
	}
	else if ((this->row == other.row) && (this->column <= other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::operator>(const Position& other) {
	bool ret = false;

	if (this->row > other.row)
	{
		ret = true;
	}
	else if ((this->row == other.row) && (this->column > other.column))
	{
		ret = true;
	}

	return ret;
}

bool Position::operator>=(const Position& other) {
	bool ret = false;

	if (this->row > other.row)
	{
		ret = true;
	}
	else if ((this->row == other.row) && (this->column >= other.column))
	{
		ret = true;
	}

	return ret;
}

#if 0

#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
	Position position;
	cout << position.GetRow() << ", "
		<< position.GetColumn()
		<< endl;
	
	Position next = position.OffsetRight();
	cout << next.GetRow() << ", "
		<< next.GetColumn()
		<< endl;

	next = next.OffsetLeft(2);
	cout << next.GetRow() << ", "
		<< next.GetColumn()
		<< endl;

	next = next.OffsetDown(3);
	cout << next.GetRow() << ", "
		<< next.GetColumn()
		<< endl;

	next = next.OffsetUp(2);
	cout << next.GetRow() << ", "
		<< next.GetColumn()
		<< endl;

	Position copy(next);
	cout << copy.GetRow() << ", "
		<< copy.GetColumn()
		<< endl;

	if (copy.IsEqual(next))
	{
		cout << "is equal."
			<< endl;
	}

	if (copy.IsNotEqual(position))
	{
		cout << "is not equal."
			<< endl;
	}

	if (position.IsLesserThan(copy))
	{
		cout << "is lesser than"
			<< endl;
	}

	if (copy.IsGreaterThan(position))
	{
		cout << "is greater than"
			<< endl;
	}

	if (copy == next)
	{
		cout << "=="
			<< endl;
	}

	if (copy != position)
	{
		cout << "!="
			<< endl;
	}

	if (position < copy)
	{
		cout << "<"
			<< endl;
	}

	if (copy <= copy)
	{
		cout << "<="
			<< endl;
	}

	if (copy > position)
	{
		cout << ">"
			<< endl;
	}

	if (copy >= copy)
	{
		cout << ">="
			<< endl;
	}

	return 0;
}

#endif