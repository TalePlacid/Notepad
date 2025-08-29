#include <afxwin.h>
#include "HistoryBook.h"

#pragma warning(disable:4996)

HistoryBook::HistoryBook(Long capacity)
	:histories(capacity), undoneHistories(capacity) {
	this->top = 0;
	this->capacity = capacity;
	this->historyLength = 0;
	this->undoneHistoryLength = 0;
}

HistoryBook::~HistoryBook() {

}

HistoryBook::HistoryBook(const HistoryBook& source)
	:histories(source.histories), undoneHistories(source.undoneHistories) {
	this->top = &this->histories.GetTop()->GetElement();
	this->capacity = source.capacity;
	this->historyLength = source.historyLength;


}

HistoryBook& HistoryBook::operator=(const HistoryBook& source) {
	this->histories = source.histories;
	this->top = &this->histories.GetTop()->GetElement();
	this->capacity = source.capacity;
	this->historyLength = source.historyLength;

	return *this;
}

History* HistoryBook::Push(History history) {
	DropOldestStack<History>::Node* node = this->histories.Push(history);
	this->top = &node->GetElement();
	if (this->historyLength < this->capacity)
	{
		(this->historyLength)++;
	}

	return &node->GetElement();
}

History HistoryBook::Pop() {
	DropOldestStack<History>::Node node = this->histories.Pop();
	
	DropOldestStack<History>::Node* link = this->histories.GetTop();
	if (link != 0)
	{
		this->top = &link->GetElement();
	}
	else
	{
		this->top = 0;
	}
	(this->historyLength)--;

	return node.GetElement();
}

bool HistoryBook::IsEmpty() {
	return this->historyLength <= 0;
}