#ifndef _SUBJECT_H //guard
#define _SUBJECT_H
#include "Array.h"
#include <string>
using namespace std;

typedef signed long int Long;

class Observer;

class Subject {
public:
	Subject(Long capacity = 32);
	virtual ~Subject() = 0;

	virtual Long Register(Observer* observer);
	virtual Long Unregister(Observer* observer);
	virtual void Notify(string interest);
private:
	Array<Observer*> observers;
	Long capacity;
	Long length;
};

int CompareObservers(void* one, void* other);

#endif // !_SUBJECT_H
