#include "MultiByteWrappingPolicy.h"
#include "Glyph.h"

#pragma warning(disable:4996)

MultiByteWrappingPolicy::MultiByteWrappingPolicy() {

}

MultiByteWrappingPolicy::~MultiByteWrappingPolicy() {

}

Long MultiByteWrappingPolicy::FindWrappingPoint(Glyph* row, Long overflowedIndex) {
	return overflowedIndex - 1;
}