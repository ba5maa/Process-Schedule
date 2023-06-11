#pragma once
#ifndef PriQue_h
#define PriQue_h

#include <iostream>
#include "Queue.h"
#include "Process.h"
using namespace std;

template <class Type>
class PriQue : 
	public Queue<Type>
{
private:

	// Overloading OStream operator
    friend ostream& operator<<(ostream&, const PriQue&);

public:
    PriQue();
    ~PriQue();

    virtual bool enqueue(const Type& newEntry, int pri);	// OverRide the enqueue of regular Queue

};

#endif // !PriQue_h

template<class Type>
inline PriQue<Type>::PriQue()
{
}

template<class Type>
inline PriQue<Type>::~PriQue()
{
}

template<typename Type>
inline bool PriQue<Type>::enqueue(const Type& newEntry, int pri)
{
	Node<Type>* newNodePtr = new Node<Type>(newEntry, pri);
	if (this->isEmpty() || pri < this->frontPtr->Priority())	//special case if this is the first node to insert
	{
		newNodePtr->setNext(this->frontPtr);
		this->frontPtr = newNodePtr;
	}
	else
	{
		Node<Type>* ptr = this->frontPtr;
		while (ptr->Next() && ptr->Next()->Priority() >= pri)
		{
			ptr = ptr->Next();
		}
		newNodePtr->setNext(ptr->Next());
		ptr->setNext(newNodePtr);
	}
	this->size++;
	return true;
}

