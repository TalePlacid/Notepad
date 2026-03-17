#include "MultiByteWidthCache.h"
#include "NonAsciiCharacterMeasurer.h"

#pragma warning(disable:4996)

MultiByteWidthCache::MultiByteWidthCache(CWnd* parent, Long capacity)
	:multiByteWidths(capacity) {
	this->parent = parent;
	this->capacity = capacity;
	this->length = 0;
}

MultiByteWidthCache::~MultiByteWidthCache() {

}

Long MultiByteWidthCache::GetWidth(char(*character)) {
	Long index = this->multiByteWidths.LinearSearchUnique(character, this->CompareMultiByteWidths);
	Long width;

	if (index >= 0)
	{
		width = this->multiByteWidths[index].GetWidth();
	}
	else
	{
		width = NonAsciiCharacterMeasurer::MesureWidth(this->parent, character);

		MultiByteWidth multiByteWidth(character, width);
		if (this->length < this->capacity)
		{
			this->multiByteWidths.Store(this->length, multiByteWidth);
		}
		else
		{
			this->multiByteWidths.AppendFromRear(multiByteWidth);
			(this->capacity)++;
		}
		(this->length)++;
	}

	return width;
}

int MultiByteWidthCache::CompareMultiByteWidths(void* one, void* other) {
	int isSameCharacter = 0;
	MultiByteWidth* multiByteWidth = (MultiByteWidth*)one;
	char* character = (char*)other;
	const char* cachedCharacter = multiByteWidth->GetCharacter();

	if (cachedCharacter[0] == character[0] && cachedCharacter[1] == character[1])
	{
		isSameCharacter = 1;
	}

	return isSameCharacter;
}
