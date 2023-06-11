#include "RR.h"
#include "Scheduler.h"

int RR::TSlice = -1;

RR::RR(Scheduler* skdptr) : Processor(skdptr)
{
	TimeinRUN = 0;
}

RR::~RR()
{
}

bool RR::setTSlice(int ts)
{
	if (ts <= 0)
		return false;
	TSlice = ts;
	return true;
}

bool RR::AddtoRDY(Process* p)
{
	if (!p)
		return false;

	RDY.enqueue(p);
	exFT += p->currentCT();
	return true;
}

void RR::PutinStop()
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

void RR::Print()
{
	cout << "[RR  ]: " << RDY.getSize() << " RDY: " << RDY << '\n';
}

bool RR::RDYtoRUN()
{
	if (RUN)
		return false;

	Process* p;
	if (!RDY.isEmpty())
	{
		RDY.dequeue(p);
		RUN = p;
		exFT -= p->currentCT();
		p->setStateOfProcess(RUNP);
		State = BUSY;
		return true;
	}
	return false;
}

bool RR::RUNtoRDY()
{
	if (RDY.enqueue(RUN))
	{
		RUN = NULL;
		State = IDLE;
		return true;
	}
	return false;
}

bool RR::ScheduleAlgo(int ts)
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

	bool x;
	if (State == IDLE)
	{
		IDLEtime++;
		x = true;
		while (x)
		{
			if (RDYtoRUN())
				x = SkdPtr->ProcessMigrationfromRRtoSJF(RUN, this, ts);
			else
				break;
		}
		if (RUN)
		{
			RUN->setRT(ts);
			RUN->Execute(this, ts);

		}
		b = true;
	}
	else if (State == BUSY)
	{
		BUSYtime++;
		if (TSlice == TimeinRUN)
		{
			RDY.enqueue(RUN);
			exFT += RUN->currentCT();
			RUN = nullptr;
			State = IDLE;
			TimeinRUN = 0;
		}
		else
		{
			RUN->Execute(this, ts);
			TimeinRUN++;
		}
		b = true;
	}
	return b;
}

int RR::getexFT()
{
	if (RUN)
		return exFT + RUN->currentCT();
	return exFT;
}

Process* RR::STEAL()
{
	Process* prc;
	if (!RDY.isEmpty())
	{
		RDY.dequeue(prc);
		exFT -= prc->currentCT();
		return prc;
	}
	return nullptr;
}


ProcType RR::getType()
{
	return RRP;
}

void RR::setTimeinRun(int x)
{
	TimeinRUN = x;
}

