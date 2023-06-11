#include "EDF.h"
#include "Scheduler.h"

EDF::EDF(Scheduler* skdptr) : Processor(skdptr)
{
}

EDF::~EDF()
{
}

bool EDF::ScheduleAlgo(int ts)
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
		if (!RDY.isEmpty())
			if (RUN->getDL() > RDY.peekFront()->getDL())
			{
				if (RDY.enqueue(RUN, RUN->getDL()))
				{
					exFT += RUN->currentCT();
					if (RDY.dequeue(RUN))
						exFT -= RUN->currentCT();
					else
					{
						State = IDLE;
						RUN = NULL;
					}
				}

			}
			else
				RUN->Execute(this, ts);
		else
			RUN->Execute(this, ts);

		b = true;
	}
	return b;
}

int EDF::getexFT()
{
	if (isBUSY())
		return exFT + RUN->currentCT();
	return exFT;
}



bool EDF::AddtoRDY(Process* p)
{
	if (!p)
		return false;

	RDY.enqueue(p, p->getDL());
	exFT += p->currentCT();
	return true;
}

void EDF::PutinStop()
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

void EDF::Print()
{
	cout << "[EDF ]: " << RDY.getSize() << " RDY: " << RDY << '\n';
}

Process* EDF::STEAL()
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

bool EDF::RDYtoRUN()
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

bool EDF::RUNtoRDY()
{
	if (!RUN)
		return false;
	if (RDY.enqueue(RUN, RUN->getDL()))
	{
		RUN = NULL;
		State = IDLE;
		return true;
	}
	return false;
}


ProcType EDF::getType()
{
	return EDFP;
}