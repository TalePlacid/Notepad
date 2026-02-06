#ifndef _NOTEPOSITIONRESOLVER_H //guard
#define _NOTEPOSITIONRESOLVER_H
#include <afxwin.h>

typedef signed long int Long;

class NotePositionResolver {
public:
	NotePositionResolver(CWnd* parent);
	~NotePositionResolver();

	void PointToNotePosition(CPoint point, Long& rowIndex, Long& columnIndex);
private:
	CWnd* parent;
};

#endif // !_NOTEPOSITIONRESOLVER_H
