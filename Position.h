#ifndef _POSITION_H //guard
#define _POSITION_H

typedef signed long int Long;

class Position {
public:
	Position(Long row = 0, Long column = 0);
	~Position();
	Position(const Position& source);
	Position& operator=(const Position& source);

	Position Right(Long distance = 1);
	Position Left(Long distance = 1);
	Position Up(Long distance = 1);
	Position Down(Long distance = 1);
	Position Move(Long row, Long column);

	bool IsEqual(const Position& other);
	bool IsNotEqual(const Position& other);
	bool IsLesserThan(const Position& other);
	bool IsGreaterThan(const Position& other);

	Long GetRow() const;
	Long GetColumn() const;

	bool operator==(const Position& other);
	bool operator!=(const Position& other);
	bool operator<(const Position& other);
	bool operator<=(const Position& other);
	bool operator>(const Position& other);
	bool operator>=(const Position& other);
private:
	Long row;
	Long column;
};

inline Long Position::GetRow() const {
	return this->row;
}

inline Long Position::GetColumn() const {
	return this->column;
}

#endif // !_POSITION_H
