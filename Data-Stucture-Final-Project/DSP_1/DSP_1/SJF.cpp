#include "SJF.h"
#include "Scheduler.h"

SJF::SJF(Scheduler* skdptr) : Processor(skdptr)
{
}

SJF::~SJF()
{
}

bool SJF::ScheduleAlgo(int ts)
{
	bool b = false;
	if (isSTOP())
	{
		STOPtime++;
		timeStoped++;
		if (timeStoped >= StopTime)
		{
			State = IDLE;
			return b;
		}
		if (RUN || !RDY.isEmpty())
			b = true;
	}
	else if (rand() % 100 < StopProb)
	{
		PutinStop();
		return b;
	}

	if (State == IDLE)
	{
		IDLEtime++;
		if (RDY.dequeue(RUN))
		{
			State = BUSY;
			exFT -= RUN->currentCT();
			RUN->setRT(ts);
			RUN->Execute(this, ts);
			b = true;
		}
	}
	else if (State == BUSY)
	{
		BUSYtime++;
		RUN->Execute(this, ts);
		b = true;
	}
	return b;
}

int SJF::getexFT()
{
	if (RUN)
		return exFT + RUN->currentCT();
	return exFT;
}



bool SJF::AddtoRDY(Process* p)
{
	if (!p)
		return false;

	RDY.enqueue(p, p->getAT());
	exFT += p->currentCT();
	return true;
}

void SJF::PutinStop()
{
	if (StopTime <= 0)
		return;
	
	State = STOP;
	timeStoped = 0;

	if (RUN)
		if (SkdPtr->Add2aRDY(RUN))
			RUN = NULL;
		else
			return;

	Process* p;
	while (!RDY.isEmpty())
		if (SkdPtr->Add2aRDY(RDY.peekFront()))
		{
			RDY.dequeue(p);
			exFT -= p->currentCT();
		}
		else
			return;

}

void SJF::Print()
{
	cout << "[SJF ]: " << RDY.getSize() << " RDY: " << RDY << '\n';
}

Process* SJF::STEAL()
{
	Process* pcr;
	if (!RDY.isEmpty())
	{
		RDY.dequeue(pcr);
		exFT -= pcr->currentCT();
		return pcr;
	}
	return nullptr;
}

bool SJF::RDYtoRUN()
{
	if (RUN)
		return false;

	if (RDY.dequeue(RUN))
	{
		State = BUSY;
		return true;
	}
	return false;
}

bool SJF::RUNtoRDY()
{
	if (!RUN)
		return false;
	if (RDY.enqueue(RUN, RUN->getAT()))
	{
		RUN = NULL;
		State = IDLE;
		return true;
	}
	return false;
}


ProcType SJF::getType()
{
	return SJFP;
}