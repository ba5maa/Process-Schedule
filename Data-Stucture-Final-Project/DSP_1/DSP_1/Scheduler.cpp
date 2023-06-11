#include "Scheduler.h"

Scheduler::Scheduler() : 
	CPU(NULL)
{
	NF = 0;  NS = 0;   NR = 0; 	NE = 0;
	RTF = 0;  MaxW = 0;  STL = 0;

	CPUsize = 0;
	CPUindex = 0;

	ProcessCount = 0;
	maxPID = 0;

	RTFnum = 0;
	MaxWnum = 0;

	UIptr = new UI(this);
}

Scheduler::~Scheduler()
{
	Process* temp{};
	while (!TRM.isEmpty())
	{
		TRM.dequeue(temp);
		delete temp;
	}

	for (int i = 0; i < CPUsize; i++)
		delete CPU[i];
	delete CPU;

	delete UIptr;
}

void Scheduler::Simulate()
{
	if (!LoadInput(""))
		return;
	makeCPU();
	UIptr->inOutputMode();
	srand((unsigned)time(NULL) * 0);
	
	bool b;
	int TS = 0;
	while (true)
	{
		TS++;
		b = false;
		for (int i = 0; i < CPUsize; i++)
			b = CPU[i]->ScheduleAlgo(TS) || b;

		b = BLKtoRDY() || b;
		b = NEWtoRDY(TS) || b;
		Steal(TS);
		UIptr->UpdateInterface(TS);

		if (ProcessCount <= 0)
			break;
	}

	MakeOutputFile();
	UIptr->SimulationEnded();
}

bool Scheduler::AddtoBLK(Process* p)
{
	if (p)
		if (BLK.enqueue(p))
		{
			p->setStateOfProcess(BLKP);
			return true;
		}
	return false;
}

bool Scheduler::AddtoTRM(Process* p)
{
	if (p)
		if (TRM.enqueue(p))
		{
			p->setStateOfProcess(TRMP);
			if (!p->isForked())
				ProcessCount--;
			return true;
		}
	return false;
}

bool Scheduler::Add2aRDY(Process* p)
{
	if (!p)
		return false;

	int idx = -1;
	int minFT = INT_MAX;
	for (int i = 0; i < CPUsize; i++)
		if (CPU[i]->getexFT() < minFT && !CPU[i]->isSTOP())
		{
			minFT = CPU[i]->getexFT();
			idx = i;
		}

	if (idx == -1)
		return false;

	p->setStateOfProcess(RDYP);
	return CPU[idx]->AddtoRDY(p);
}

bool Scheduler::LoadInput(string fileName)
{
	if (fileName == "") 
	{
		fileName = UIptr->inFileName();
	}

	ifstream inputFile;
	inputFile.open("Files\\" + fileName);

	if (!inputFile.is_open()) 
	{
		UIptr->PrintMsg("Error! Can't open file.");
		return false;
	}

	int RRtslice;
	int forkP = 0;
	int ProNum;
	int StopT = 0;
	int StopP = 0;

	NF = 0;
	inputFile >> NF >> NS >> NR >> NE;
	inputFile >> RRtslice;
	inputFile >> RTF >> MaxW >> STL >> forkP;
	inputFile >> StopT >> StopP;
	inputFile >> ProNum;

	RR::setTSlice(RRtslice);
	FCFS::setForkP(forkP);
	Processor::setStopTime(StopT);
	Processor::setStopProb(StopP);

	for (int i = 0; i < ProNum; i++) 
	{
		Process* p = new Process;
		p->readInfo(inputFile);
		NEW.enqueue(p);

		ProcessCount++;
		if (p->getPID() > maxPID)
			maxPID = p->getPID();
	}

	if (NF > 0) 
	{
		KillSignal ks{};
		while (!inputFile.eof())
		{
			ks.readInfo(inputFile);
			FCFS::AddKillSig(ks);
		}
	}

	inputFile.close();

	return true;
}

void Scheduler::makeCPU()
{
	if (CPU)
		return;

	CPUsize = NF + NS + NR + NE;
	CPU = new Processor * [CPUsize];

	int k = 0;

	for (int i = 0; i < NF; i++)
		CPU[k++] = new FCFS(this);

	for (int i = 0; i < NS; i++)
		CPU[k++] = new SJF(this);

	for (int i = 0; i < NR; i++)
		CPU[k++] = new RR(this);
	
	for (int i = 0; i < NE; i++)
		CPU[k++] = new EDF(this);
}

void Scheduler::PrintCPU()
{
	for (int i = 0; i < CPUsize; i++)
	{
		cout << "Processer " << i+1 << " ";
		CPU[i]->Print();
	}
}

void Scheduler::PrintCPU_RUN()
{
	int runCount = 0;
	for (int i = 0; i < CPUsize; i++)
		if (CPU[i]->isBUSY())
			runCount++;

	cout << runCount << " RUN:";
	for (int i = 0; i < CPUsize; i++)
	{
		if (CPU[i]->isBUSY())
		{
			cout << ' ';
			CPU[i]->PrintRUN();
			cout << "(P" << i+1 << "),";
		}
	}
	cout << "\b \n";
}

void Scheduler::PrintBLK()
{
	cout << BLK.getSize() << " BLK: " << BLK << '\n';
}

void Scheduler::PrintTRM()
{
	cout << TRM.getSize() << " TRM: " << TRM << '\n';
}

bool Scheduler::NEWtoRDY(int ts)
{
	if (NEW.isEmpty())
		return false;
	Process* ptr;
	while (!NEW.isEmpty()) {
		if (NEW.peekFront()->getAT() > ts)
			return true;
		NEW.dequeue(ptr);
		Add2aRDY(ptr);
	}
	return true;
}

bool Scheduler::Add2anFCFS(Process* p)
{
	if (!p)
		return false;

	int idx = -1;
	int minFT = INT_MAX;
	for (int i = 0; i < NF; i++)
		if (CPU[i]->getexFT() < minFT)
		{
			minFT = CPU[i]->getexFT();
			idx = i;
		}

	if (idx == -1)
		return false;

	p->setStateOfProcess(RDYP);
	return CPU[idx]->AddtoRDY(p);
}

bool Scheduler::Kill(int pid, int ts)
{
	for (int i = 0; i < NF; i++)
		if (CPU[i]->TerminatebyPID(pid, ts))
			return true;
	return false;
}

bool Scheduler::BLKtoRDY()
{
	/*Process* temp;
	temp = BLK.peekFront();
	if (BLK.isEmpty() == false) {
		temp->getIO();
		int x = temp->getcountoftime();
		int y = temp->getIO_D();
		if (y >= x)
		{
			BLK.dequeue(temp);
			CPUindex %= CPUsize;
			CPU[CPUindex++]->AddtoRDY(temp);
			temp->dequeueBLK();
			return true;
		}
		else
		{
			temp->increment();
			return false;
		}
		return temp != NULL;
	}
	return false;*/

	Process* temp = NULL;
	if (!BLK.isEmpty())
		if (!BLK.peekFront()->handleIO())
			if (BLK.dequeue(temp))
				return Add2aRDY(temp);
	return false;
}

void Scheduler::MakeOutputFile()
{
	ofstream file("Files\\test-out.txt");

	file << setw(6) << left << "TT" << setw(6) << left << "PID" << setw(6) << left << "AT" << setw(6) << left << "CT"
		<< setw(6) << left << "IO_D" << '\t' << setw(6) << left << "WT" << setw(6) << left << "RT" << setw(6) << left << "TRT" << '\n';

	int TotWT = 0;
	int TotRT = 0;
	int TotTRT = 0;
	int killed = 0;
	int forked = 0;
	int befDL = 0;
	float count = 0;
	float TotUtilz = 0;
	Process* p{};
	Processor* P{};
	while (TRM.dequeue(p))
	{
		p->PrintToFile(file);
		TotWT += p->getWT();
		TotRT += p->getRT();
		TotTRT += p->getTRT();
		count++;

		if (p->isKilled())
			killed++;
		if (p->isForked())
			forked++;
		if (p->getDL() - p->getTT() >= 0)
			befDL++;
		delete p;
	}


	file << "Processes: " << count << '\n';

	file << "Avg WT = " << setprecision((int)log10(TotWT / count) + 3) << TotWT / count << ",\t";
	file << "Avg RT = " << setprecision((int)log10(TotRT / count) + 3) << TotRT / count << ",\t";
	file << "Avg TRT = " << setprecision((int)log10(TotTRT / count) + 3) << TotTRT / count << '\n';
	
	file << "Migration%:\t";
	file << "RTF = " << setprecision((int)log10(RTFnum / count) + 3) << RTFnum / count * 100<< "%,\t";
	file << "MaxW = " << setprecision((int)log10(MaxWnum / count) + 3) << MaxWnum / count * 100 << "%\n";
	
	file << "Work Steal%: " << stealcount / count << '%' << '\n';
	file << "Forked Process: " << setprecision((int)log10(forked / count) + 3) << forked / count * 100 << '%' << '\n';
	file << "Killed Process: " << setprecision((int)log10(killed / count) + 3) << killed / count * 100 << '%' << '\n';
	file << "Before DeadLine: " << setprecision((int)log10(befDL / count) + 3) << befDL / count * 100 << '%' << '\n';
	
	file << "\nProcessors:" << CPUsize << " ";
	file << "[" << NF << " FCFS, " << NS << " SJF, " << NR << " RR, " << NE << " EDF]" << '\n';
	
	file << "Processor Load" << "\n";
	for (int i = 0; i < CPUsize; i++)
	{
		file << "P" << i + 1 << " = " << setprecision((int)log10((float)CPU[i]->getBUSYtime() / TotTRT) + 3) << (float)CPU[i]->getBUSYtime() / TotTRT * 100 << '%' << '\t';
	}

	file << "\n\nProceesor Utiliz\n";
	for (int i = 0; i < CPUsize; i++)
	{
		file << "P" << i + 1 << " = " << setprecision((int)log10((float)CPU[i]->getBUSYtime() / (CPU[i]->getIDLEtime() + CPU[i]->getBUSYtime() + CPU[i]->getSTOPtime())) + 3) <<
			(float)CPU[i]->getBUSYtime() / (CPU[i]->getIDLEtime() + CPU[i]->getBUSYtime() + CPU[i]->getSTOPtime()) * 100 << '%' << '\t';
	}

	file << "\nAvg Utilization = ";
	for (int i = 0; i < CPUsize; i++)
	{
		TotUtilz += (float)CPU[i]->getBUSYtime() / (CPU[i]->getIDLEtime() + CPU[i]->getBUSYtime());
	}
	file << TotUtilz / CPUsize * 100 << '%';

	file.close();
}

void Scheduler::set_LQF_SQF()
{
	int max = 0;
	int min = INT_MAX;
	for (int i = 0; i < CPUsize; i++)
	{
		if (!CPU[i]->isSTOP())
		{
			if (CPU[i]->getexFT() > max)
			{
				max = CPU[i]->getexFT();
				LQidx = i;
			}
			if (CPU[i]->getexFT() < min)
			{
				min = CPU[i]->getexFT();
				SQidx = i;
			}
		}

	}

	LQ = max;
	SQ = min;
}

float Scheduler::getSTLlimit()
{
	return (float)(LQ - SQ) / LQ;
}

void Scheduler::Steal(int ts)
{
	set_LQF_SQF();
	if (ts % STL == 0 && LQ > 0 && SQ >= 0)
	{
		while (getSTLlimit() > 0.4)
		{
			Process* prc = CPU[LQidx]->STEAL();

			if (!prc)
				return;
			CPU[SQidx]->AddtoRDY(prc);
			LQ = CPU[LQidx]->getexFT();
			SQ = CPU[SQidx]->getexFT();
			stealcount++;
		}
	}
}


bool Scheduler::ProcessMigrationfromRRtoSJF(Process* p, Processor* processor, int timestep)
{
	if (p->getStateofProcess() == RUNP)
	{

		int timetofinish = p->rmngCT(); // remaining cpu time 
		if (timetofinish < RTF)
		{
			int x = getshortestRDYSJF();
			if (x >= 0)
			{
				CPU[x]->AddtoRDY(p);
				processor->setRUN(NULL);
				processor->setStateofprocessor(IDLE);
				RTFnum++; // for output file 
				return true;
			}
			// we have to set TimeinRUN = 0; for RR processors 
		}
		return false;
	}
	return false;
}

bool Scheduler::ProcessMigrationfromFCFStoRR(Process* p, Processor* processor, int timestep)
{
	if (p->getStateofProcess() == RUNP && !p->isForked()) // has to not be forked 
	{
		//timestep- arrivaltime- runtime for process so far
		int WaitingTime = timestep - (p->getAT()) - (p->gettimeserved());
		if (WaitingTime > MaxW)
		{
			int x = getshoertestRDYRR();
			if (x >= 0)
			{
				CPU[x]->AddtoRDY(p);
				processor->setRUN(NULL);
				processor->setStateofprocessor(IDLE);
				MaxWnum++;
				return true;
			}
		}
		return false;
	}
	return false;
}

int Scheduler::getshoertestRDYRR()
{
	int idx = 0;
	int minFT = INT_MAX;
	int totalnoOfProcessor = NF + NS + NR;
	for (int i = 0; i < totalnoOfProcessor; i++)
	{
		if (CPU[i]->getType() == RRP)
		{
			if (CPU[i]->getexFT() < minFT)
			{
				minFT = CPU[i]->getexFT();
				idx = i;
			}
		}

	}
	if (CPU[idx]->getType() == RRP) {
		return idx;
	}
	else
		return -1;
}

int Scheduler::getshortestRDYSJF()
{
	int idx = 0;
	int minFT = INT_MAX;
	int totalnoOfProcessor = NF + NS + NR;
	for (int i = 0; i < totalnoOfProcessor; i++)
	{
		if (CPU[i]->getType() == SJFP)
		{
			if (CPU[i]->getexFT() < minFT)
			{
				minFT = CPU[i]->getexFT();
				idx = i;
			}
		}
	}
	if (CPU[idx]->getType() == SJFP)
	{
		return idx;
	}
	else
		return -1;
}
