#include <afxwin.h>
#include "HistoryBook.h"
#include "Command.h"
#include "MacroCommand.h"
#include "resource.h"
#include "FindReplaceOption.h"

#pragma warning(disable:4996)

HistoryBook::HistoryBook(Long capacity)
	:histories(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

HistoryBook::~HistoryBook() {
	while (!this->histories.IsEmpty())
	{
		DropOldestStack<Command*>::Node node = this->histories.Pop();
		delete node.GetElement();
	}
}

HistoryBook::HistoryBook(const HistoryBook& source)
	:histories(source.capacity), latestPushTime(source.latestPushTime) {
	DropOldestStack<Command*>::Node* it = source.histories.GetBottom();
	while (it == 0)
	{
		this->histories.Push(it->GetElement()->Clone());
		it = it->GetNext();
	}

	this->capacity = source.capacity;
	this->length = source.length;
}

HistoryBook& HistoryBook::operator=(const HistoryBook& source) {
	this->histories = source.histories;
	DropOldestStack<Command*>::Node* it = source.histories.GetBottom();
	while (it == 0)
	{
		this->histories.Push(it->GetElement()->Clone());
		it = it->GetNext();
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->latestPushTime = source.latestPushTime;

	return *this;
}

Command* HistoryBook::Bind(Command* command) {
	Command* history = command;
	DateTime now = DateTime::Now();
	DateTime latest = this->latestPushTime.AddSeconds(1);
	if ((this->latestPushTime == DateTime() || DateTime::Now() <= this->latestPushTime.AddSeconds(1))
		&& this->histories.Peek() != 0)
	{
		DropOldestStack<Command*>::Node top = this->histories.Pop();
		(this->length)--;
		Command* top_ = top.GetElement();
		if (command->GetId() == top_->GetId())
		{
			if (top_->IsMacroCommand())
			{
				top_->Add(command);
				history = top_;
			}
			else
			{
				history = new MacroCommand(top_->GetParent());
				history->Add(top_);
				history->Add(command);
			}
		}
		else
		{
			this->histories.Push(top_);
			if (this->length < this->capacity)
			{
				(this->length)++;
			}
		}
	}

	return history;
}

Command** HistoryBook::Push(Command* history) {
	DropOldestStack<Command*>::Node* node = this->histories.Push(history);
	if (this->length < this->capacity)
	{
		(this->length)++;
	}
	this->latestPushTime = DateTime::Now();

	return &node->GetElement();
}

Command* HistoryBook::Pop() {
	DropOldestStack<Command*>::Node node = this->histories.Pop();
	(this->length)--;

	return node.GetElement();
}

bool HistoryBook::IsEmpty() {
	return this->length <= 0;
}

Command** HistoryBook::Clear() {
	while (!this->histories.IsEmpty())
	{
		DropOldestStack<Command*>::Node node = this->histories.Pop();
		delete node.GetElement();
	}

	Command** history = 0;
	DropOldestStack<Command*>::Node* node = this->histories.Clear();
	if (node != 0)
	{
		history = &node->GetElement();
	}

	this->length = 0;

	return history;
}

Command** HistoryBook::Peek() {
	Command** top = 0;
	DropOldestStack<Command*>::Node* node = this->histories.Peek();
	if (node != 0)
	{
		top = &node->GetElement();
	}

	return top;
}