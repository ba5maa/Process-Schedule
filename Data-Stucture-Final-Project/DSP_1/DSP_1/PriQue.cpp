#include "PriQue.h"

ostream& operator<<(ostream& out, const PriQue<Process*>& PQ)
{
	if (PQ.isEmpty())
		return out;

	Node<Process*>* temp = PQ.frontPtr;
	out << *temp->Item();
	temp = temp->Next();

	while (temp) {
		out << ", " << *temp->Item();
		temp = temp->Next();
	}

	return out;
}
