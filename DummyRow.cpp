#include "DummyRow.h"

#pragma warning(disable:4996)

DummyRow::DummyRow(Long capacity, bool isDirty)
	:Row(capacity, isDirty) {

}

DummyRow::~DummyRow() {

}

DummyRow::DummyRow(const DummyRow& source)
	:Row(source) {

}

DummyRow& DummyRow::operator=(const DummyRow& source) {
	Row::operator=(source);

	return *this;
}

Glyph* DummyRow::Clone() {
	return new DummyRow(*this);
}

bool DummyRow::IsDummyRow() {
	return true;
}