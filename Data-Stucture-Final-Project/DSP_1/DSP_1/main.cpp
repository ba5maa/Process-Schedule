#include <iostream>
using namespace std;

#include "Node.h"
#include "LinkedList.h"
#include "Queue.h"
#include "PriQue.h"
#include "Process.h"
#include "Scheduler.h"

int main(void)
{
	Scheduler S;
	S.Simulate();

	system("pause>nul");
	return 0;
}