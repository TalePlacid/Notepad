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
		Node() {
			this->previous = 0;
			this->next = 0;
		}

		Node(T element) {
			this->previous = 0;
			this->element = element;
			this->next = 0;
		}

		Node(Node* previous, T element) {
			this->previous = previous;
			this->element = element;
			this->next = 0;
		}

		Node(T element, Node* next) {
			this->previous = 0;
			this->element = element;
			this->next = next;
		}

		~Node() {

		}

		Node(const Node& source) {
			this->previous = source.previous;
			this->element = source.element;
			this->next = source.next;
		}

		Node& operator=(const Node& source) {
			this->previous = source.previous;
			this->element = source.element;
			this->next = source.next;

			return *this;
		}

		T& GetElement() const {
			return const_cast<T&>(this->element);
		}
	private:
		Node* previous;
		T element;
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

	DropOldestStack(const DropOldestStack& source) {
		Node* node = 0;
		Node* sourceCurrent = source.bottom;
		while (sourceCurrent != 0)
		{
			if (sourceCurrent != source.bottom)
			{
				node = new Node(this->top, sourceCurrent->GetElement());
				this->top->next = node;
				this->top = node;
			}
			else
			{
				node = new Node(sourceCurrent->GetElement());
				this->bottom = node;
				this->top = node;
			}
			sourceCurrent = sourceCurrent->next;
		}

		this->capacity = source.capacity;
		this->length = source.length;
	}

	DropOldestStack& operator=(const DropOldestStack& source) {
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
				node = new Node(this->top, sourceCurrent->GetElement());
				this->top->next = node;
				this->top = node;
			}
			else
			{
				node = new Node(sourceCurrent->GetElement());
				this->bottom = node;
				this->top = node;
			}
			sourceCurrent = sourceCurrent->next;
		}

		this->capacity = source.capacity;
		this->length = source.length;

		return *this;
	}

	Node* Push(T element) {
		Node* node = 0;
		if (this->length < this->capacity)
		{
			if (this->bottom != 0)
			{
				node = new Node(this->top, element);
				this->top->next = node;
				this->top = node;
			}
			else
			{
				node = new Node(element);
				this->bottom = node;
				this->top = node;
			}

			(this->length)++;
		}
		else
		{
			node = new Node(this->top, element);
			this->top->next = node;
			this->top = node;

			node = this->bottom;
			this->bottom = this->bottom->next;
			delete node;
		}

		return this->top;
	}

	Node Pop() {
		Node* link = this->top;

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

		Node node = *link;
		if (link != NULL)
		{
			delete link;
		}

		return node;
	}

	Node& Peek() {
		return *(this->top);
	}

	bool IsEmpty() {
		return this->length <= 0;
	}

	Node* GetTop() const{
		return const_cast<Node*>(this->top);
	}

	Node* GetBottom() const {
		return const_cast<Node*>(this->bottom);
	}

	Long GetCapacity() const {
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
