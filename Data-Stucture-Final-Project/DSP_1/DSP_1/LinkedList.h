#pragma once
#ifndef List_h
#define List_h

#include <iostream>
#include "Node.h"
using namespace std;

template <class Type>
class LinkedList
{
protected:
	Node<Type>* Head;
	int size;

	// Overloading OStream operator
	friend ostream& operator<<(ostream&, const LinkedList<Process*>&);

public:
	LinkedList();
	~LinkedList();

	bool isEmpty() const;
	int getSize() const;
	bool Find(const Type& key) const;
	int CountOccurance(const Type& value) const;
	Type getHead();

	//void Insert();
	void InsertBeg(const Type& value);
	void InsertEnd(const Type& value);

	bool Delete(int n, Type& val);
	bool Delete(const Type& value);
	bool DeleteEvery(const Type& value);
	bool DeleteFirst();
	bool DeleteFirst(Type& val);
	bool DeleteLast();
	void Clear();
	
	void Merge(const LinkedList<Type>& L);
	void Reverse();

	//bool remove();
	//void clear();

};

template <>
bool LinkedList<Process*>::Delete(int n, Process*& val);


#endif // !List_h

template<class Type>
inline LinkedList<Type>::LinkedList() : 
	Head(NULL), size(0)
{
}

template<class Type>
inline LinkedList<Type>::~LinkedList()
{
}

template<class Type>
inline bool LinkedList<Type>::isEmpty() const
{
	return size == 0;
}

template<class Type>
inline int LinkedList<Type>::getSize() const
{
	return size;
}

template<class Type>
inline bool LinkedList<Type>::Find(const Type& key) const
{
	Node<Type>* ptr = Head;
	//This is how to iterate over a linked list using for loop, NO i++ here (ptr = ptr->next)
	for (ptr; ptr; ptr = ptr->Next()) //the second ptr means while ptr!= NULL
	{
		if (ptr->Item() == key)
			return true;
	}
	return false;
}

template<class Type>
inline int LinkedList<Type>::CountOccurance(const Type& value) const
{
	int count = 0;
	Node<Type>* ptr = Head;
	for (ptr; ptr; ptr = ptr->Next()) //the second ptr means while ptr!= NULL
	{
		if (ptr->Item() == value)
			++count;
	}
	return count;
}

template<class Type>
inline Type LinkedList<Type>::getHead()
{
	if (!isEmpty())
		return Head->Item();
	return Type();

}

template<class Type>
inline void LinkedList<Type>::InsertBeg(const Type& value)
{
	Node<Type>* R = new Node<Type>(value);
	R->setNext(Head);
	Head = R;
	size++;
}

template<class Type>
inline void LinkedList<Type>::InsertEnd(const Type& value)
{
	if (!Head) //means if head=nullptr(mean an empty list)
	{
		return InsertBeg(value);
	}
	Node<Type>* newNode = new Node<Type>(value);
	newNode->setNext(nullptr); //Because the newly inserted node will be at the end
	if (!Head->Next()) //checks if the linked list consists of the head only
	{
		Head->setNext(newNode);
		size++;
		return;
	}
	Node<Type>* ptr = Head;
	while (ptr->Next())
	{
		ptr = ptr->Next(); //this loop will exit when ptr points to the last elem. of the list
	}
	ptr->setNext(newNode); //the last elem will point to the newly created node 
	size++;

}

template<class Type>
inline bool LinkedList<Type>::Delete(const Type& value)
{
	if (!Head)//in case the list is empty
		return false;
	if (!Head->Next()) //in case the list consists of only 1 node
	{
		DeleteFirst();
		return true;
	}
	Node<Type>* current = Head;
	for (current; current; current = current->Next())
	{
		if (current->Item() == value)
		{
			current->setItem(Head->Item());
			DeleteFirst();
			return true;
		}
	}
	return false;
}

template<class Type>
inline bool LinkedList<Type>::DeleteEvery(const Type& value)
{
	bool deletedAny = false;
	while (Delete(value))
	{
		deletedAny = true;
	}
	return deletedAny;
}

template<class Type>
inline bool LinkedList<Type>::DeleteFirst()
{
	if (!Head)
		return false;
	Node<Type>* newHead = Head->Next();
	delete Head;
	Head = newHead;
	size--;
	return true;
}

template<class Type>
inline bool LinkedList<Type>::DeleteFirst(Type& val)
{
	if (Head)
		val = Head->Item();
	return DeleteFirst();
}

template<class Type>
inline bool LinkedList<Type>::DeleteLast()
{
	if (!Head)
		return false;
	if (!Head->Next())
	{
		return DeleteFirst(); //as the list contains only one elem
	}
	Node<Type>* ptr = Head;
	while (ptr->Next()->Next())
	{
		ptr = ptr->Next();
	}
	delete ptr->Next();
	ptr->setNext(nullptr);
	size--;
	return true;
}

template<class Type>
inline void LinkedList<Type>::Clear()
{
	while (DeleteFirst());
}

template<class Type>
inline void LinkedList<Type>::Merge(const LinkedList<Type>& L)
{
	Node<Type>* cur = Head;
	while (cur->Next())
		cur = cur->Next();
	cur->setNext(L.Head);
	size += L.size;
}

template<class Type>
inline void LinkedList<Type>::Reverse()
{
	Node<Type>* prev = nullptr;
	while (Head)
	{
		Node<Type>* next = Head->Next();
		Head->setNext(prev);
		prev = Head;
		Head = next;
	}
	Head = prev;
}
