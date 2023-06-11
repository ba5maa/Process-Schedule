#include "Process.h"

int Process::MaxPID = 0;

Process::Process() :
	Process(-1, -1, -1, -1)
{
}

Process::Process(int par, int at, int ct, int pid) :
	Parent(par), AT(at), PID(pid), CT(ct)
{
	RT = -1; TT = 0; TRT = 0; WT = 0;

	timeServed = 0;
	Tot_IO_D = 0;
	IO_D = 0;
	IO_R = 0;

	Killed = false;

	Child_1 = -1;
	Child_2 = -1;

	if (pid > MaxPID)
		MaxPID = pid;

}

bool Process::Execute(Processor* ProcPtr, int TS)
{
	if (timeServed >= CT)
		return Terminate(ProcPtr, TS);
	if (!IO.isEmpty())
		if (IO.peekFront().R <= timeServed)
			return Block(ProcPtr);
	timeServed++;
	return true;
}

bool Process::handleIO()
{
	if (remnBLKtime < 0)
		return false;
	
	remnBLKtime--;
	return true;
}

bool Process::Block(Processor* ProcPtr)
{
	InOut io{};
	if (ProcPtr->Block(this))
		if (IO.dequeue(io))
		{
			remnBLKtime = io.D;
			Tot_IO_D += io.D;
			return true;
		}
	return false;
}

bool Process::Terminate(Processor* ProcPtr, int ts)
{
	if (!ProcPtr->Terminate(this))
		return false;
	
	TT = ts;
	TRT = TT - AT;
	WT = TRT - CT;

	if (Child_1 >= 0)
		ProcPtr->Kill(Child_1, ts);
	if (Child_2 >= 0)
		ProcPtr->Kill(Child_2, ts);

	return true;
}

bool Process::AddChild(Process* ch)
{
	if (!ch)
		return false;

	if (Child_1 < 0)
	{
		Child_1 = ch->getPID();
		return true;
	}

	if (Child_2 < 0)
	{
		Child_2 = ch->getPID();
		return true;
	}

	return false;
}

bool Process::CanHaveaChild()
{
	return (Child_1 < 0 || Child_2 < 0);
}

bool Process::isForked()
{
	return (Parent >= 0);
}

bool Process::isKilled(bool b)
{
	if (!b)
		return Killed;
	Killed = b;
	return true;
}

bool Process::setRT(int ts)
{
	if (RT >= 0)
		return false;
	RT = ts - AT;
	return true;
}

int Process::rmngCT()
{
	return CT - timeServed;
}

int Process::currentCT()
{
	if (IO.isEmpty())
		return rmngCT();

	return IO.peekFront().R - timeServed;
}

int Process::getPID() const
{
	return PID;
}

int Process::getWT() const
{
	return WT;
}

int Process::getRT() const
{
	return RT;
}

int Process::getTT() const
{
	return TT;
}

int Process::getTRT() const
{
	return TRT;
}

int Process::getAT() const
{
	return AT;
}

int Process::getDL() const
{
	return DL;
}

bool Process::readInfo(ifstream& inFile)
{
	int IOnum;
	inFile >> AT >> PID >> CT >> DL >> IOnum;

	if (PID > MaxPID)
		MaxPID = PID;

	char str[5];
	InOut io{};
	for (int i = 0; i < IOnum; i++) {
		io.readInfo(inFile);
		IO.enqueue(io);
	}
	
	inFile.getline(str, 5, '\n');
	return true;
}

void Process::PrintToFile(ofstream& outFile)
{
	outFile << setw(6) << left << TT << setw(6) << left << PID << setw(6) << left << AT << setw(6) << left << CT
		<< setw(6) << left << Tot_IO_D << '\t' << setw(6) << left << WT << setw(6) << left << RT << setw(6) << left << TRT << '\n';
}

void Process::increment()
{
	countoftime++;
	Tot_IO_D++;
}

int Process::getcountoftime()
{
	return countoftime;
}

void Process::setStateOfProcess(State s)
{
	StateOfProcess = s;
}

State Process::getStateofProcess()
{
	return StateOfProcess;
}

InOut Process::getIO()
{
	InOut io;
	io = IO.peekFront();
	IO_D = io.D;
	IO_R = io.R;
	return io;
}

int Process::gettimeserved()
{
	return timeServed;
}

int Process::getIO_R()
{
	return IO_R;
}

int Process::getIO_D()
{
	return IO_D;
}

InOut Process::dequeueBLK()
{
	InOut io;
	IO.dequeue(io);
	IO_D = io.D;
	IO_R = io.R;
	return io;
}

ostream& operator<<(ostream& out, const Process& P)
{
	out << P.PID;
	return out;
}

