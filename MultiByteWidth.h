#ifndef _MULTIBYTEWIDTH_H
#define _MULTIBYTEWIDTH_H

typedef signed long int Long;

class MultiByteWidth {
public:
	MultiByteWidth();
	MultiByteWidth(const char(*character), Long width);
	~MultiByteWidth();
	MultiByteWidth(const MultiByteWidth& source);
	MultiByteWidth& operator=(const MultiByteWidth& source);

	bool IsEqual(const MultiByteWidth& other);
	bool IsNotEqual(const MultiByteWidth& other);
	const char* GetCharacter() const;
	Long GetWidth() const;

	bool operator==(const MultiByteWidth& other);
	bool operator!=(const MultiByteWidth& other);
private:
	char character[2];
	Long width;
};

inline const char* MultiByteWidth::GetCharacter() const {
	return this->character;
}

inline Long MultiByteWidth::GetWidth() const {
	return this->width;
}

#endif // !_MULTIBYTEWIDTH_H
