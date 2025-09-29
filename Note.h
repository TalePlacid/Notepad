#ifndef _NOTE_H //guard
#define _NOTE_H
#include "Composite.h"
#include "Array.h"
#include <string>
using namespace std;

typedef signed long int Long;

class Glyph;
class Subject;

class Note : public Composite {
public:
	Note(Long capacity = 256);
	Note(string str, Long capacity = 256);
	virtual ~Note();
	Note(const Note& source);
	Note& operator=(const Note& source);

	virtual Glyph* Clone();
	virtual string MakeString();
	virtual Long Next();
	virtual Long Last();

	virtual Long MergeRows(Long index);
	virtual Long SplitRows(Long rowIndex, Long columnIndex, bool IsDummyRow = false);
};

#endif // !_NOTE_H