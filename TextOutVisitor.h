#ifndef _TEXTOUTVISITOR_H //guard
#define _TEXTOUTVISITOR_H
#include <afxwin.h>
#include "Visitor.h"

typedef signed long int Long;

class CWnd;
class Glyph;

class TextOutVisitor : public Visitor {
public:
	TextOutVisitor(CWnd* parent, CDC* dc);
	virtual ~TextOutVisitor();

	virtual void VisitRow(Glyph* row);
	virtual void VisitCharacter(Glyph* character);
private:
	CDC* dc;
	Visitor* selectionVisitor;
	Long initialX;
	Long initialY;
	Long x;
	Long y;
};

#endif // !_TEXTOUTVISITOR_H
