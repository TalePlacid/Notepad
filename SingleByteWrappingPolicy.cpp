#include "SingleByteWrappingPolicy.h"
#include "Glyph.h"

#pragma warning(disable:4996)

SingleByteWrappingPolicy::SingleByteWrappingPolicy() {

}

SingleByteWrappingPolicy::~SingleByteWrappingPolicy() {

}

Long SingleByteWrappingPolicy::FindWrappingPoint(Glyph* row, Long overflowedIndex) {
	Long index = overflowedIndex - 1;

	Glyph* character = 0;
	Long i = overflowedIndex - 1;
	if (i >= 0)
	{
		character = row->GetAt(i);
	}

	while (i >= 0 && character->IsWordCharacter())
	{
		character = row->GetAt(i);
		i--;
	}

	if (i >= 0)
	{
		index = i + 1;
	}

	return index;
}