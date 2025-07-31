#ifndef _ARRAY_H //guard
#define _ARRAY_H

typedef signed long int Long;

template <typename T>
class Array {
public :
	Array(Long capacity = 256);
	~Array();
	Array(const Array<T>& source);
	Array<T>& operator=(const Array<T>& source);

	Long Store(Long index, T object);
	Long Insert(Long index, T object);
	Long AppendFromFront(T object);
	Long AppendFromRear(T object);

	Long Delete(Long index);
	Long DeleteFromFront();
	Long DeleteFromRear();

	void Clear();
	Long Modify(Long index, T object);

	Long LinearSearchUnique(void* key, int (*compare)(void*, void*));
	void LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));
	Long BinarySearchUnique(void* key, int (*compare)(void*, void*));
	void BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));

	void SelectionSort(int (*compare)(void*, void*));
	void BubbleSort(int (*compare)(void*, void*));
	void InsertSort(int (*compare)(void*, void*));

	void Merge(const Array<T>& one, const Array<T>& other, int (*compare)(void*, void*));
	T& GetAt(Long index);

	T& operator[](Long index);
	T* operator+(Long index);

	Long GetCapacity() const;
	Long GetLength() const;

public:
	T(*front);
	Long capacity;
	Long length;
};

template <typename T>
Array<T>::Array(Long capacity) {
	this->front = new T[capacity];
	this->capacity = capacity;
	this->length = 0;
}

template <typename T>
Array<T>::~Array() {
	if (this->front != 0)
	{
		delete[] this->front;
	}
}

template <typename T>
Array<T>::Array(const Array& source) {
	this->front = new T[source.capacity];
	Long i = 0;
	while (i < source.length)
	{
		this->front[i] = source.front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& source) {
	if (this->front != 0)
	{
		delete[] this->front;
	}
	this->front = new T[source.capacity];
	Long i = 0;
	while (i < source.length)
	{
		this->front[i] = (const_cast<Array&>(source))[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

template <typename T>
Long Array<T>::Store(Long index, T object) {
	this->front[index] = object;
	(this->length)++;

	return index;
}

template <typename T>
Long Array<T>::Insert(Long index, T object) {
	T(*objects) = new T[this->capacity + 1];

	Long i = 0;
	while (i < index)
	{
		objects[i] = this->front[i];
		i++;
	}

	while (i < this->length)
	{
		objects[i + 1] = this->front[i];
		i++;
	}

	if (this->front != 0)
	{
		delete[] this->front;
	}

	this->front = objects;
	(this->capacity)++;

	this->front[index] = object;
	(this->length)++;

	return index;
}

template <typename T>
Long Array<T>::AppendFromFront(T object) {
	T(*objects) = new T[this->capacity + 1];

	Long index = 0;
	Long i = 0;
	while (i < this->length)
	{
		objects[i + 1] = this->front[i];
		i++;
	}

	if (this->front != 0)
	{
		delete[] this->front;
	}

	this->front = objects;
	(this->capacity)++;

	this->front[index] = object;
	(this->length)++;

	return index;
}

template <typename T>
Long Array<T>::AppendFromRear(T object) {
	T(*objects) = new T[this->capacity + 1];

	Long i = 0;
	while (i < this->length)
	{
		objects[i] = this->front[i];
		i++;
	}

	if (this->front != 0)
	{
		delete[] this->front;
	}

	this->front = objects;
	(this->capacity)++;

	Long index = this->capacity - 1;
	this->front[index] = object;
	(this->length)++;

	return index;
}

template <typename T>
Long Array<T>::Delete(Long index) {
	T(*objects) = 0;
	if (this->capacity > 1)
	{
		objects = new T[this->capacity - 1];
	}

	Long i = 0;
	while (i < index)
	{
		objects[i] = this->front[i];
		i++;
	}

	i = index + 1;
	while (i < this->length)
	{
		objects[i - 1] = this->front[i];
		i++;
	}

	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}

	if (this->capacity > 1)
	{
		this->front = objects;
	}

	(this->length)--;
	(this->capacity)--;

	index = -1;

	return index;
}

template <typename T>
Long Array<T>::DeleteFromFront() {
	T(*objects) = 0;
	if (this->capacity > 1)
	{
		objects = new T[this->capacity - 1];
	}

	Long i = 1;
	while (i < this->length)
	{
		objects[i - 1] = this->front[i];
		i++;
	}

	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}

	if (this->capacity > 1)
	{
		this->front = objects;
	}

	(this->length)--;
	(this->capacity)--;

	Long index = -1;

	return index;
}

template <typename T>
Long Array<T>::DeleteFromRear() {
	T(*objects) = 0;
	if (this->capacity > 1)
	{
		objects = new T[this->capacity - 1];
	}

	Long i = 0;
	while (i < this->length - 1)
	{
		objects[i] = this->front[i];
		i++;
	}

	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}

	if (this->capacity > 1)
	{
		this->front = objects;
	}

	(this->length)--;
	(this->capacity)--;

	Long index = -1;

	return index;
}

template <typename T>
void Array<T>::Clear() {
	if (this->front != 0)
	{
		delete[] this->front;
		this->front = 0;
	}
	this->capacity = 0;
	this->length = 0;
}

template <typename T>
Long Array<T>::Modify(Long index, T object) {
	this->front[index] = object;

	return index;
}

template <typename T>
Long Array<T>::LinearSearchUnique(void* key, int(*compare)(void*, void*)) {
	Long index = -1;
	Long i = 0;
	while (i < this->length && compare(this->front + i, key) == 0)
	{
		i++;
	}

	if (i < this->length)
	{
		index = i;
	}

	return index;
}

template <typename T>
void Array<T>::LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int (*compare)(void*, void*)) {
	(*count) = 0;
	(*indexes) = new Long[this->length];

	Long i = 0;
	Long j = 0;
	while (i < this->length)
	{
		if (compare(this->front + i, key) == 0)
		{
			(*indexes)[j] = i;
			j++;
			(*count)++;
		}
		i++;
	}
}

template <typename T>
Long Array<T>::BinarySearchUnique(void* key, int (*compare)(void*, void*)) {
	Long index = -1;
	Long front = 0;
	Long rear = this->length - 1;
	Long middle = (front + rear) / 2;

	while (front <= rear && compare(this->front+middle, key) != 0)
	{
		if (compare(this->front+middle, key) < 0)
		{
			front = middle + 1;
		}
		else
		{
			rear = middle - 1;
		}

		middle = (front + rear) / 2;
	}

	if (front <= rear)
	{
		index = middle;
	}

	return index;
}

template <typename T>
void Array<T>::BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int (*compare)(void*, void*)) {
	(*count) = 0;
	(*indexes) = new Long[this->length];

	Long front = 0;
	Long rear = this->length - 1;
	Long middle = (front + rear) / 2;

	while (front <= rear && compare(this->front + middle, key) != 0)
	{
		if (compare(this->front + middle, key) < 0)
		{
			front = middle + 1;
		}
		else
		{
			rear = middle - 1;
		}
		middle = (front + rear) / 2;
	}

	Long j = middle - 1;
	while (j >= 0 && compare(this->front + j, key) == 0)
	{
		j--;
	}

	Long i = 0;
	j++;
	while (j < this->length && compare(this->front + j, key) == 0)
	{
		(*indexes)[i] = j;
		i++;
		(*count)++;
		j++;
	}
}

template <typename T>
void Array<T>::SelectionSort(int (*compare)(void*, void*)) {
	T object;
	Long minIndex;
	Long i = 0;
	Long j;

	while (i < this->length - 1)
	{
		minIndex = i;
		j = i + 1;
		while (j < this->length)
		{
			if (compare(this->front+j, this->front+minIndex) < 0)
			{
				minIndex = j;
			}
			j++;
		}
		object = this->front[i];
		this->front[i] = this->front[minIndex];
		this->front[minIndex] = object;
		i++;
	}
}

template <typename T>
void Array<T>::BubbleSort(int (*compare)(void*, void*)) {
	T object;
	Long i = 0;
	Long j;
	Long flag = 0;

	while (i < this->length && flag == 0)
	{
		flag = 1;
		j = 0;
		while (j < this->length - i - 1)
		{
			if (compare(this->front + j, this->front + j + 1) > 0)
			{
				object = this->front[j];
				this->front[j] = this->front[j + 1];
				this->front[j + 1] = object;
				flag = 0;
			}
			j++;
		}
		i++;
	}
}

template <typename T>
void Array<T>::InsertSort(int (*compare)(void*, void*)) {
	T object;
	Long i = 1;
	Long j;

	while (i < this->length)
	{
		object = this->front[i];
		j = i - 1;
		while (j >= 0 && compare(this->front + j, &object) > 0)
		{
			this->front[j + 1] = this->front[j];
			j--;
		}
		this->front[j + 1] = object;
		i++;
	}
}

template <typename T>
void Array<T>::Merge(const Array<T>& one, const Array<T>& other, int (*compare)(void*, void*)) {
	Long arraySize = one.GetLength() + other.GetLength();

	if (this->front != 0)
	{
		delete[] this->front;
	}
	this->front = new T[arraySize];

	Long i = 0;
	Long j = 0;
	Long k = 0;
	while (i < one.GetLength() && j < other.GetLength())
	{
		if ((const_cast<Array&>(one))[i] < (const_cast<Array&>(other))[j])
		{
			this->front[k] = (const_cast<Array&>(one))[i];
			i++;
		}
		else
		{
			this->front[k] = (const_cast<Array&>(other))[j];
			j++;
		}
		k++;
	}

	while (i < one.GetLength())
	{
		this->front[k] = (const_cast<Array&>(one))[i];
		k++;
		i++;
	}

	while (j < other.GetLength())
	{
		this->front[k] = (const_cast<Array&>(other))[j];
		k++;
		j++;
	}

	this->capacity = arraySize;
	this->length = arraySize;
}

template <typename T>
T& Array<T>::GetAt(Long index) {
	return this->front[index];
}

template <typename T>
T& Array<T>::operator[](Long index) {
	return const_cast<T&>(this->front[index]);
}

template <typename T>
T* Array<T>::operator+(Long index) {
	return this->front + index;
}

template <typename T>
inline Long Array<T>::GetCapacity() const {
	return this->capacity;
}

template <typename T>
inline Long Array<T>::GetLength() const {
	return this->length;
}


#endif // !_ARRAY_H
