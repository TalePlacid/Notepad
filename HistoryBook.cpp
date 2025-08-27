#include <afxwin.h>
#include "HistoryBook.h"

#pragma warning(disable:4996)

HistoryBook::HistoryBook(Long capacity)
	:histories(capacity) {
	this->top = 0;
	this->capacity = capacity;
	this->length = 0;
}

HistoryBook::~HistoryBook() {

}

HistoryBook::HistoryBook(const HistoryBook& source)
	:histories(source.histories) {
	this->top = &this->histories.GetTop()->GetElement();
	this->capacity = source.capacity;
	this->length = source.length;
}

HistoryBook& HistoryBook::operator=(const HistoryBook& source) {
	this->histories = source.histories;
	this->top = &this->histories.GetTop()->GetElement();
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

History* HistoryBook::Push(History history) {
	DropOldestStack<History>::Node* node = this->histories.Push(history);

	return &node->GetElement();
}

History HistoryBook::Pop() {
	DropOldestStack<History>::Node node = this->histories.Pop();
	
	return node.GetElement();
}

bool HistoryBook::IsEmpty() {
	return this->length <= 0;
}