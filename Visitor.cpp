#include <afxwin.h>
#include "Visitor.h"

#pragma warning(disable:4996)

Visitor::Visitor(CWnd* parent) {
	this->parent = parent;
}

Visitor::~Visitor() {

}

void Visitor::VisitGlyph(Glyph* glyph) {

}

void Visitor::VisitComposite(Glyph* composite) {

}

void Visitor::VisitRow(Glyph* row) {

}

void Visitor::VisitCharacter(Glyph* character) {

}