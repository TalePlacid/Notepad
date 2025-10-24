#ifndef _TEXTOUTVISITOR_H //guard
#define _TEXTOUTVISITOR_H
#include <afxwin.h>
#include "Visitor.h"

typedef signed long int Long;

class Glyph;

class TextOutVisitor : public Visitor {
public:
	TextOutVisitor(CWnd* parent, CDC* dc);
	virtual ~TextOutVisitor();

	virtual void VisitRow(Glyph* row);
	virtual void VisitCharacter(Glyph* character);

	Long GetLogicalX() const;
	Long GetLogicalY() const;
private:
	CDC* dc;
	Visitor* selectionVisitor;
	Long logicalX;
	Long logicalY;
	Long drawingX;
	Long drawingY;
	RECT drawingArea;
};

inline Long TextOutVisitor::GetLogicalX() const {
	return this->logicalX;
}

inline Long TextOutVisitor::GetLogicalY() const {
	return this->logicalY;
}

#endif // !_TEXTOUTVISITOR_H
