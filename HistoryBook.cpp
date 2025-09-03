#include <afxwin.h>
#include "HistoryBook.h"

#pragma warning(disable:4996)

HistoryBook::HistoryBook(Long capacity)
	:histories(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

HistoryBook::~HistoryBook() {

}

HistoryBook::HistoryBook(const HistoryBook& source)
	:histories(source.histories) {
	this->capacity = source.capacity;
	this->length = source.length;
}

HistoryBook& HistoryBook::operator=(const HistoryBook& source) {
	this->histories = source.histories;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

History* HistoryBook::Push(History history) {
	DropOldestStack<History>::Node* node = this->histories.Push(history);
	if (this->length < this->capacity)
	{
		(this->length)++;
	}

	return &node->GetElement();
}

History HistoryBook::Pop() {
	DropOldestStack<History>::Node node = this->histories.Pop();
	(this->length)--;

	return node.GetElement();
}

bool HistoryBook::IsEmpty() {
	return this->length <= 0;
}

History* HistoryBook::Clear() {
	History* history = NULL;
	DropOldestStack<History>::Node* node = this->histories.Clear();
	if (node != NULL)
	{
		history = &node->GetElement();
	}

	this->length = 0;

	return history;
}