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

	Long GetXOffset() const;
	Long GetYOffset() const;
private:
	CDC* dc;
	Visitor* selectionVisitor;
	RECT paintingArea;
	Long xOffset;
	Long yOffset;
	Long x;
	Long y;
};

inline Long TextOutVisitor::GetXOffset() const {
	return this->xOffset;
}

inline Long TextOutVisitor::GetYOffset() const {
	return this->yOffset;
}

#endif // !_TEXTOUTVISITOR_H
