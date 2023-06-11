#pragma once
#ifndef Node_h
#define Node_h

#include <iostream>
using namespace std;

class Process;

template <class Type>
class Node
{
private:
	Type item;
	Node<Type>* next;
	int priority;

public:
	Node();	//
	Node(const Type& newItem, Node<Type>* nextPtr = NULL, int pri = 0);	// Constructor Node<>(item) 
	Node(const Type newItem, int pri, Node<Type>* nextPtr = NULL);		// Constructor Node<>(item, priority)

	Type Item() const;										// Return item
	Node* Next() const;										// Return next
	int Priority() const;									// return priority

	void setItem(Type newItem);								// Set item
	void setNext(Node<Type>* newNext);						// Set next
	void setPriority(int pri);								// Set priority

};

#include "Node.cpp"

#endif // !Node_h