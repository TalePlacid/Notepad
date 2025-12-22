#ifndef _NOTEWRAPPER_H //guard
#define _NOTEWRAPPER_H

typedef signed long int Long;

class CWnd;
class Glyph;

class NoteWrapper {
public:
	NoteWrapper(CWnd* parent);
	~NoteWrapper();

	Long Wrap();
	Long Rewrap();
	Long Unwrap();
private:
	CWnd* parent;
};

#endif // !_NOTEWRAPPER_H
