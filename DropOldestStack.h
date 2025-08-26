#ifndef _DROPOLDESTSTACK_H
#define _DROPOLDESTSTACK_H

typedef signed long int Long;

template <typename T>
class DropOldestStack {
public:
	//Nested Class
	class Node {
		friend class DropOldestStack;
	public:
		Node(T object) {
			this->previous = 0;
			this->object = object;
			this->next = 0;
		}

		Node(Node* previous, T object) {
			this->previous = previous;
			this->object = object;
			this->next = 0;
		}

		Node(T object, Node* next) {
			this->previous = 0;
			this->object = object;
			this->next = next;
		}

		~Node() {

		}

		T& GetObject() const {
			return const_cast<T&>(this->object);
		}
	private:
		Node* previous;
		T object;
		Node* next;
	};

public:
	DropOldestStack(Long capacity = 128) {
		this->top = 0;
		this->bottom = 0;
		this->capacity = capacity;
		this->length = 0;
	}

	~DropOldestStack() {
		Node* previous = 0;
		Node* current = this->bottom;
		while (current != 0)
		{
			previous = current;
			current = current->next;
			delete previous;
		}
	}

	DropOldestStack(DropOldestStack& source) {
		Node* node = 0;
		Node* sourceCurrent = source.bottom;
		while (sourceCurrent != 0)
		{
			if (sourceCurrent != source.bottom)
			{
				node = new Node(this->top, sourceCurrent->GetObject());
				this->top->next = node;
				this->top = node;
			}
			else
			{
				node = new Node(sourceCurrent->GetObject());
				this->bottom = node;
				this->top = node;
			}
			sourceCurrent = sourceCurrent->next;
		}

		this->capacity = source.capacity;
		this->length = source.length;
	}

	DropOldestStack& operator=(DropOldestStack& source) {
		Node* previous = 0;
		Node* current = this->bottom;
		while (current != 0)
		{
			previous = current;
			current = current->next;
			delete previous;
		}
		
		Node* node = 0;
		Node* sourceCurrent = source.bottom;
		while (sourceCurrent != 0)
		{
			if (sourceCurrent != source.bottom)
			{
				node = new Node(this->top, sourceCurrent->GetObject());
				this->top->next = node;
				this->top = node;
			}
			else
			{
				node = new Node(sourceCurrent->GetObject());
				this->bottom = node;
				this->top = node;
			}
			sourceCurrent = sourceCurrent->next;
		}

		this->capacity = source.capacity;
		this->length = source.length;

		return *this;
	}

	Node* Push(T object) {
		Node* node = 0;
		if (this->length < this->capacity)
		{
			if (this->bottom != 0)
			{
				node = new Node(this->top, object);
				this->top->next = node;
				this->top = node;
			}
			else
			{
				node = new Node(object);
				this->bottom = node;
				this->top = node;
			}

			(this->length)++;
		}
		else
		{
			node = new Node(this->top, object);
			this->top->next = node;
			this->top = node;
			
			node = this->bottom;
			this->bottom = this->bottom->next;
			delete node;
		}

		return this->top;
	}

	Node Pop() {
		Node* node = this->top;

		this->top = this->top->previous;
		if (this->top != 0)
		{
			this->top->next = 0;
		}
		else
		{
			this->bottom = 0;
		}
		(this->length)--;

		return *node;
	}

	Node& peek() {
		return *(this->top);
	}

	bool IsEmpty() {
		return this->length <= 0;
	}

	Long GetCapacity() const{
		return this->capacity;
	}

	Long GetLength() const {
		return this->length;
	}
private:
	Node* top;
	Node* bottom;
	Long capacity;
	Long length;
};

#endif // !_DROPOLDESTSTACK_H
