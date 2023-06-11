#include "FCFS.h"
#include "Scheduler.h"

//static int ForkP = 0;
int FCFS::ForkP = 0;
Queue<KillSignal> FCFS::KillSig{};

FCFS::FCFS(Scheduler* skdptr) : Processor(skdptr)
{
}

FCFS::~FCFS()
{
}

bool FCFS::setForkP(int pro)
{
	if (pro < 0)
		return false;

	ForkP = pro;
	return true;
}

bool FCFS::AddKillSig(KillSignal KS)
{
	return KillSig.enqueue(KS);
}

bool FCFS::ScheduleAlgo(int ts) // returns true if state of processor is busy 
{
	bool b = false; // returns if the processor is busy or idle
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
	else if (rand() % 100 < StopProb)  // put a processor state in stop 
	{
		PutinStop();
		return b;
	}


	KillSignal ks{};
	while (!KillSig.isEmpty())
		if (KillSig.peekFront().KTime < ts)
			KillSig.dequeue(ks);
		else
			break;

	if (KillSig.peekFront().KTime == ts)
		if (TerminatebyPID(KillSig.peekFront().PID, ts, true))
			KillSig.dequeue(ks);

	bool x; // for migration 
	if (State == IDLE)
	{
		IDLEtime++; // for output file 
		x = true;
		while (x)
		{
			if (RDYtoRUN()) // true if a process went to run 
			{
				x = SkdPtr->ProcessMigrationfromFCFStoRR(RUN, this, ts); // true if migration occured 
			}
			else
				break;
		}
		if (RUN)
		{
			RUN->setRT(ts); // response time, the time waited in RDY for the first time 
			RUN->Execute(this, ts);

		}
		b = true;

	}
	else if (isBUSY())
	{
		BUSYtime++; // for output file 
		static int ForkP;
		if (RUN->CanHaveaChild())
			if (rand() % 100 <= ForkP)
			{
				Process* p = new Process(RUN->getPID(), ts, RUN->rmngCT());
				RUN->AddChild(p);
				SkdPtr->Add2anFCFS(p);
			}

		// migration from fcfs to rr if > maxW 
		RUN->Execute(this, ts);
		b = true;
	}
	return b;
}

int FCFS::getexFT()
{
	if (RUN)
		return exFT + RUN->currentCT();
	return exFT;
}

ProcType FCFS::getType()
{
	return FCFSP;
}

bool FCFS::AddtoRDY(Process* p)
{
	if (!p)
		return false;
	
	RDY.InsertEnd(p);
	exFT += p->currentCT();
	return true;
}

void FCFS::PutinStop()
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
	while (RDY.DeleteFirst(p))
		if (!SkdPtr->Add2aRDY(p))
		{
			RDY.InsertBeg(p);
			return;
		}
		else
			exFT -= p->currentCT();

}

void FCFS::Print()
{
	cout << "[FCFS]: " << RDY.getSize() << " RDY: " << RDY << '\n';
}

Process* FCFS::STEAL()
{
	if (!RDY.isEmpty())
	{
		Process* prc = RDY.getHead();
		if (!prc->isForked())
		{
			RDY.DeleteFirst();
			exFT -= prc->currentCT();
			return prc;
		}
	}
	return nullptr;
}

bool FCFS::RDYtoRUN()
{
	if (RUN)
		return false;
	Process* p;
	if (RDY.DeleteFirst(p))
	{
		RUN = p;
		State = BUSY;
		exFT -= RUN->currentCT();
		p->setStateOfProcess(RUNP);
		return true;
	}
	return false;
}

bool FCFS::RUNtoRDY()
{
	RDY.InsertEnd(RUN);
	RUN = NULL;
	State = IDLE;
	return true;
}

bool FCFS::TerminatebyPID(int pid, int ts, bool kill)
{
	if (RUN)
		if (RUN->getPID() == pid)
		{
			if (kill)
				RUN->isKilled(true);
			RUN->Terminate(this, ts);
		}

	Process* p{};
	if (RDY.Delete(pid, p))
	{
		exFT -= p->currentCT();
		if (kill)
			p->isKilled(true);
		return p->Terminate(this, ts);
	}
	return false;
}