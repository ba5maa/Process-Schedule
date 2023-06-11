#include "Processor.h"
#include "Scheduler.h"

int Processor::StopTime = -1;
int Processor::StopProb = -1;

Processor::Processor(Scheduler* skdptr) : 
	SkdPtr(skdptr)
{
	RUN = NULL;
	State = IDLE;
	exFT = 0;

	timeStoped = 0;
	SkdPtr = skdptr;

	BUSYtime = 0;
	IDLEtime = 0;
}

Processor::~Processor()
{
}

bool Processor::setStopTime(int st)
{
	if (st > 0)
		if (StopTime <= 0)
		{
			StopTime = st;
			return true;
		}
	return false;
}

bool Processor::setStopProb(int sp)
{
	if (sp >= 0)
		if (StopProb <= 0)
		{
			StopProb = sp;
			return true;
		}
	return false;
}

bool Processor::Block(Process* p)
{
	if (!p)
		return false;

	bool temp = SkdPtr->AddtoBLK(p);
	if (RUN == p && temp)
	{
		RUN = NULL;
		State = IDLE;
	}
	return temp;
}

bool Processor::Terminate(Process* p)
{
	if (!p)
		return false;

	bool temp = SkdPtr->AddtoTRM(p);
	if (RUN == p && temp)
	{
		RUN = NULL;
		State = IDLE;
	}
	return temp;
}

bool Processor::Kill(int pid, int ts)
{
	return SkdPtr->Kill(pid, ts);
}

bool Processor::isIDLE()
{
	return State == IDLE;
}

bool Processor::isBUSY()
{
	return (State == BUSY && RUN);
}

bool Processor::isSTOP()
{
	return State == STOP;
}

int Processor::getexFT()
{
	return exFT;
}

void Processor::PrintRUN()
{
	cout << RUN->getPID();
}

Process* Processor::STEAL()
{
	return nullptr;
}

int Processor::getIDLEtime()
{
	return IDLEtime;
}

int Processor::getBUSYtime()
{
	return BUSYtime;
}

int Processor::getSTOPtime()
{
	return STOPtime;
}



void Processor::setRUN(Process* p)
{
	RUN = p;
}

Process* Processor::getRUN()
{
	return RUN;
}

void Processor::setStateofprocessor(ProcState S)
{
	State = S;
}

ProcState Processor::getstateofprocessor()
{
	return State;
}



bool Processor::BlockRUN()
{
	return Block(RUN);
}

bool Processor::TerminatebyPID(int pid, int ts, bool kill)
{
	return false;
}

bool Processor::TerminateRUN()
{
	return Terminate(RUN);
}
