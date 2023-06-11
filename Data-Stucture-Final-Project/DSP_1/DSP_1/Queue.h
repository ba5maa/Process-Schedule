#pragma once
#ifndef Queue_h
#define Queue_h

#include <iostream>
#include "Node.h"
using namespace std;

class Process;

template <class Type>
class Queue
{
protected:
	int size;
	Node<Type>* backPtr;
	Node<Type>* frontPtr;

	// Overloading OStream operator
	friend ostream& operator<<(ostream&, const Queue<Process*>&);

public:
	Queue();
	~Queue();

	virtual bool isEmpty() const;	// Return true if the Queue is Empty
	virtual int getSize() const;	// Return size of the Queue

	virtual bool enqueue(const Type item);	// Add item to the End of the Queue
	virtual bool dequeue(Type& item);		// Remove item at the Front of the Queue
	virtual Type dequeue();

	virtual Type peekFront() const;	// Return the item in Front

	virtual void Clear();			// Clear the Queue

};

#endif // !Queue_h

template<class Type>
inline Queue<Type>::Queue() :
	size(0), backPtr(NULL), frontPtr(NULL)
{
}

template<class Type>
inline Queue<Type>::~Queue()
{
	Clear();
}

template<class Type>
inline bool Queue<Type>::isEmpty() const
{
	return size == 0;
}

template<class Type>
inline int Queue<Type>::getSize() const
{
	return size;
}

template<class Type>
inline bool Queue<Type>::enqueue(const Type item)
{
	Node<Type>* temp = new Node<Type>(item);
	if (isEmpty())
	{
		frontPtr = temp;
		backPtr = temp;
		size++;
		return true;
	}
	backPtr->setNext(temp);
	backPtr = temp;
	size++;
	return true;
}

template<class Type>
inline bool Queue<Type>::dequeue(Type& item)
{
	if (isEmpty())
		return false;

	item = frontPtr->Item();
	Node <Type>* temp = frontPtr;
	frontPtr = frontPtr->Next();
	delete temp;

	if (frontPtr == NULL)
		backPtr = NULL;

	size--;
	return true;
}

template<class Type>
inline Type Queue<Type>::dequeue()
{
	Type item{};
	dequeue(item);
	return item;
}


template<class Type>
inline Type Queue<Type>::peekFront() const
{
	if (!isEmpty())
		return frontPtr->Item();
	return Type();
}

template<class Type>
inline void Queue<Type>::Clear()
{
	Type temp;
	while (dequeue(temp));
}

