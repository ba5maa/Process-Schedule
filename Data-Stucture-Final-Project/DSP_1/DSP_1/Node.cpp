#ifndef Node_cpp
#define Node_cpp

#include "Node.h"

template<class Type>
Node<Type>::Node() :
	next(NULL), priority(0)
{
}

template<class Type>
Node<Type>::Node(const Type& newItem, Node<Type>* nextPtr, int pri) :
	item(newItem), next(nextPtr), priority(pri)
{
}

template<class Type>
Node<Type>::Node(const Type newItem, int pri, Node<Type>* nextPtr) :
	item(newItem), priority(pri), next(nextPtr)
{
}

template<class Type>
Type Node<Type>::Item() const
{
	return item;
}

template<class Type>
Node<Type>* Node<Type>::Next() const
{
	return next;
}

template<class Type>
int Node<Type>::Priority() const
{
	return priority;
}

template<class Type>
void Node<Type>::setItem(Type newItem)
{
	item = newItem;
}

template<class Type>
void Node<Type>::setNext(Node<Type>* newNext)
{
	next = newNext;
}

template<class Type>
void Node<Type>::setPriority(int pri)
{
	priority = pri;
}

#endif // !Node_cpp
