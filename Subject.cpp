#include "Subject.h"
#include "Observer.h"

#pragma warning(disable:4996)

Subject::Subject(Long capacity)
	:observers(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

Subject::~Subject() {

}

Long Subject::Register(Observer* observer) {
	Long index;
	if (this->length < this->capacity)
	{
		index = this->observers.Store(this->length, observer);
	}
	else
	{
		index = this->observers.AppendFromRear(observer);
		(this->capacity)++;
	}
	(this->length)++;

	return index;
}

Long Subject::Unregister(Observer* observer) {
	Long index = this->observers.LinearSearchUnique(observer, CompareObservers);
	index = this->observers.Delete(index);

	return index;
}

void Subject::Notify(string interest) {
	Long i = 0;
	while (i < this->length)
	{
		this->observers[i]->Update(this, interest);
		i++;
	}
}

int CompareObservers(void* one, void* other) {
	return one == other;
}


