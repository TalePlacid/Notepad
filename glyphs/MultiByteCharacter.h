#ifndef _MULTIBYTECHARACTER_H //guard
#define _MULTIBYTECHARACTER_H
#include "Character.h"

class MultiByteCharacter : public Character {
public:
	MultiByteCharacter();
	MultiByteCharacter(char(*content), bool isSelected = false);
	virtual ~MultiByteCharacter();
	MultiByteCharacter(const MultiByteCharacter& source);
	MultiByteCharacter& operator=(const MultiByteCharacter& source);
	MultiByteCharacter& operator=(const char(*content));

	virtual Glyph* Clone();
	virtual string MakeString();
	virtual bool IsWordCharacter();
	virtual bool IsMultiByteCharacter();

	bool IsEqual(const MultiByteCharacter& other);
	bool IsEqual(const char(*content));
	bool IsNotEqual(const MultiByteCharacter& other);
	bool IsNotEqual(const char(*content));

	bool operator==(const MultiByteCharacter& other);
	bool operator==(const char(*content));
	bool operator!=(const MultiByteCharacter& other);
	bool operator!=(const char(*content));

	virtual operator char*() const;

	char* GetContent() const;
private:
	char content[2];
};

inline char* MultiByteCharacter::GetContent() const {
	return const_cast<char*>(this->content);
}

#endif // !_MULTIBYTECHARACTER_H
