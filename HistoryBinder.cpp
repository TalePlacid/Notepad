#include "HistoryBinder.h"

#pragma warning(disable:4996)

HistoryBinder::HistoryBinder() {

}

HistoryBinder::~HistoryBinder() {

}

Long HistoryBinder::Bind(Command* command) {
	Long index = this->history.Add(command->Clone());
	this->bindingStartTime = Date::Now();

	return index;
}

History HistoryBinder::Commit() {
	History history = this->history;
	this->bindingStartTime = Date();
	this->history = History();

	return history;
}

bool HistoryBinder::IsBindable(Command* command) {
	bool isBindable = false;

	if (this->history.GetLength() == 0)
	{
		isBindable = true;
	}
	else if (this->history.GetLength() > 0)
	{
		Long index = this->history.GetLength() - 1;
		Command* previous = this->history[index];
		if (command->GetId() == previous->GetId() &&
			(this->bindingStartTime == Date(const_cast<char*>("1900-01-01 00:00:00")) 
				|| Date::Now() <= this->bindingStartTime.AddSeconds(1)))
		{
			isBindable = true;
		}
	}

	return isBindable;
}