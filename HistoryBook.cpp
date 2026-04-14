#include <afxwin.h>
#include "HistoryBook.h"
#include "resource.h"
#include "FindReplaceOption.h"
#include "commands/Command.h"
#include "commands/MacroCommand.h"

#pragma warning(disable:4996)

HistoryBook::HistoryBook(Long capacity)
	:histories(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->isBindingStopped = false;
}

HistoryBook::~HistoryBook() {
	while (!this->histories.IsEmpty())
	{
		DropOldestStack<Command*>::Node node = this->histories.Pop();
		delete node.GetElement();
	}
}

HistoryBook::HistoryBook(const HistoryBook& source)
	:histories(source.capacity), bindingStartTime(source.bindingStartTime) {
	DropOldestStack<Command*>::Node* it = source.histories.GetBottom();
	while (it != 0)
	{
		this->histories.Push(it->GetElement()->Clone());
		it = it->GetNext();
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->isBindingStopped = source.isBindingStopped;
}

HistoryBook& HistoryBook::operator=(const HistoryBook& source) {
	this->histories = source.histories;
	DropOldestStack<Command*>::Node* it = source.histories.GetBottom();
	while (it != 0)
	{
		this->histories.Push(it->GetElement()->Clone());
		it = it->GetNext();
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->isBindingStopped = source.isBindingStopped;
	this->bindingStartTime = source.bindingStartTime;

	return *this;
}

Command* HistoryBook::Bind(Command* command) {
	Command* history = command;
	if (this->isBindingStopped)
	{
		this->isBindingStopped = false;
	}
	else
	{
		DateTime now = DateTime::Now();
		DateTime bindingEndTime = this->bindingStartTime.AddSeconds(1);
		if ((this->bindingStartTime == DateTime() || now <= bindingEndTime)
			&& this->histories.Peek() != 0)
		{
			DropOldestStack<Command*>::Node top = this->histories.Pop();
			(this->length)--;
			Command* top_ = top.GetElement();
			if (command->GetID() == top_->GetID())
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
	}

	return history;
}

void HistoryBook::StopBinding() {
	this->isBindingStopped = true;
}

Command** HistoryBook::Push(Command* history) {
	if (this->length >= this->capacity)
	{
		DropOldestStack<Command*>::Node* bottom = this->histories.GetBottom();
		if (bottom->GetElement() != 0)
		{
			delete bottom->GetElement();
			bottom->GetElement() = 0;
		}
	}
	
	DropOldestStack<Command*>::Node* node = this->histories.Push(history);
	if (this->length < this->capacity)
	{
		(this->length)++;
	}

	if (!history->IsMacroCommand())
	{
		this->bindingStartTime = DateTime::Now();
	}

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
	this->isBindingStopped = false;

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