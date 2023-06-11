#include "Queue.h"
#include "Process.h"

ostream& operator<<(ostream& out, const Queue<Process*>& Q)
{
	// TODO: insert return statement here
	if (Q.isEmpty())
		return out;

	Node<Process*>* temp = Q.frontPtr;
	out << *temp->Item();
	temp = temp->Next();

	while (temp != NULL) {
		out << ", " << *temp->Item();
		temp = temp->Next();
	}

	return out;
}
