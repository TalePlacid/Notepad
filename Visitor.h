#ifndef _VISITOR_H //guard
#define _VISITOR_H

class CWnd;
class Glyph;

class Visitor {
public:
	Visitor(CWnd* parent);
	virtual ~Visitor() = 0;

	virtual void VisitGlyph(Glyph* glyph);
	virtual void VisitComposite(Glyph* composite);
	virtual void VisitNote(Glyph* note);
	virtual void VisitRow(Glyph* row);
	virtual void VisitCharacter(Glyph* Character);
protected:
	CWnd* parent;
};


#endif // !_VISITOR_H

