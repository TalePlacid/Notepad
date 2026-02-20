#ifndef _SINGLEBYTECHARACTER_H //guard
#define _SINGLEBYTECHARACTER_H
#include "Character.h"
#include <string>
using namespace std;

class SingleByteCharacter : public Character {
public:
	SingleByteCharacter(char content = ' ', bool isSelected = false);
	virtual ~SingleByteCharacter();
	SingleByteCharacter(const SingleByteCharacter& source);
	SingleByteCharacter& operator=(const SingleByteCharacter& source);
	SingleByteCharacter& operator=(const char content);

	virtual Glyph* Clone();
	virtual string MakeString();
	virtual bool IsWordCharacter();

	bool IsEqual(const SingleByteCharacter& other);
	bool IsEqual(const char content);
	bool IsNotEqual(const SingleByteCharacter& other);
	bool IsNotEqual(const char content);

	bool operator==(const SingleByteCharacter& other);
	bool operator==(const char content);
	bool operator!=(const SingleByteCharacter& other);
	bool operator!=(const char content);

	virtual operator char*() const;

	char GetContent() const;
private:
	char content;
};

inline char SingleByteCharacter::GetContent() const {
	return this->content;
}

#endif // !_SINGLEBYTECHARACTER_H