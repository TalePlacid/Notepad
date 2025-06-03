#ifndef _NOTE_H //guard
#define _NOTE_H
#include "Composite.h"
#include "Array.h"
#include "Observer.h"
#include <string>
using namespace std;

typedef signed long int Long;

class Glyph;
class Subject;

class Note : public Composite, public Observer {
public:
	Note(Long capacity = 128);
	Note(string str, Long capacity = 128);
	virtual ~Note();
	Note(const Note& source);
	Note& operator=(const Note& source);

	virtual Glyph* Clone();
	virtual string MakeString();
	virtual Long First();
	virtual Long Previous();

	virtual void Update(Subject* subject, string interest);
};

#endif // !_NOTE_H