#ifndef _NOTEWRAPPER_H //guard
#define _NOTEWRAPPER_H

typedef signed long int Long;

class CWnd;

class NoteWrapper {
public:
	NoteWrapper(CWnd* parent);
	~NoteWrapper();

	void InsertDummyRows();
	void DeleteDummyRows();
private:
	CWnd* parent;
};


#endif // !_NOTEWRAPPER_H
