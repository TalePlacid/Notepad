#ifndef _PAINTINGVISITOR_H
#define _PAINTINGVISITOR_H
#include <afxwin.h>
#include "Visitor.h"

typedef signed long int Long;

class PaintingVisitor : public Visitor {
public:
	PaintingVisitor(CWnd* parent, CDC* dc);
	virtual ~PaintingVisitor();

	virtual void VisitRow(Glyph* row);
	virtual void VisitCharacter(Glyph* character);

	Long GetLogicalX() const;
	Long GetLogicalY() const;
private:
	CDC* dc;
	CFont* oldFont;
	Long logicalX;
	Long logicalY;
	RECT drawingArea;
};

inline Long PaintingVisitor::GetLogicalX() const {
	return this->logicalX;
}

inline Long PaintingVisitor::GetLogicalY() const {
	return this->logicalY;
}

#endif // !_PAINTINGVISITOR_H
