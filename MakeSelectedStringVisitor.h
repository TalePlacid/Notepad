#ifndef _MAKESELECTEDSTRINGVISITOR_H //guard
#define _MAKESELECTEDSTRINGVISITOR_H
#include <afxwin.h>
#include "Visitor.h"

typedef signed long int Long;

class Glyph;

class MakeSelectedStringVisitor : public Visitor {
public:
	MakeSelectedStringVisitor(CWnd* parent);
	virtual ~MakeSelectedStringVisitor();

	virtual void VisitRow(Glyph* row);
	virtual void VisitCharacter(Glyph* character);

	CString& GetSelectedString() const;
private:
	CString selectedString;
	BOOL hasSelectedCharacter;
	Long rowIndex;
};

inline CString& MakeSelectedStringVisitor::GetSelectedString() const {
	return const_cast<CString&>(this->selectedString);
}

#endif // !_MAKESELECTEDSTRINGVISITOR_H
