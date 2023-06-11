#include "LinkedList.h"
#include "Process.h"


ostream& operator<<(ostream& out, const LinkedList<Process*>& L)
{
	if (L.isEmpty())
		return out;

	Node<Process*>* temp = L.Head;
	out << *temp->Item();
	temp = temp->Next();

	while (temp)
	{
		out << ", " << *temp->Item();
		temp = temp->Next();
	}

	return out;
}

template <>
bool LinkedList<Process*>::Delete(int n, Process*& val)
{
	if (!Head || isEmpty())
		return false;

	Node<Process*>* walker = Head;
	if (Head->Item()->getPID() == n)
	{
		Head = Head->Next();
		val = walker->Item();
		delete walker;
		size--;
		return true;
	}
	while (walker->Next())
	{
		if (walker->Next()->Item()->getPID() == n)
		{
			Node<Process*>* temp = walker->Next();
			walker->setNext(temp->Next());
			val = temp->Item();
			delete temp;
			size--;
			return true;
		}
		walker = walker->Next();
	}
	return false;
}
