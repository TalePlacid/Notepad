#ifndef _OBSERVER_H //guard
#define _OBSERVER_H
#include <string>
using namespace std;

class Subject;

class Observer {
public:
	Observer();
	virtual ~Observer() = 0;

	virtual void Update(Subject *subject, string interest) = 0;
};

#endif // !_OBSERVER_H
