#pragma once
#ifndef Scheduler_h
#define Scheduler_h

#include "Process.h"
#include "Processor.h"
#include "FCFS.h"
#include "SJF.h"
#include "EDF.h"
#include "RR.h"
#include "UI.h"
#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>
using namespace std;

class Scheduler
{
private:
	int NF, NS, NR, NE;		// NF: no.of FCFS Proc, NS: no.of SJF Proc, NR: no.of RR Proc
	int RTF, MaxW, STL;	// 
	int maxPID;			// Maximum PID (only PH1)

	Queue<Process*> NEW;	// New Processes
	Queue<Process*> BLK;	// Blocked Processes
	Queue<Process*> TRM;	// Terminated Processes

	Queue<KillSignal> SigKill;

	Processor** CPU;	// Array of Pointers to the Processores
	int CPUsize;		// Size of CPU array
	int CPUindex;		// Index of the next Processor to take a Process

	int ProcessCount;	// Number of Processes created in the System

	UI* UIptr;	// Pointer to UI(User Interface)

	int SQ;
	int LQ;
	int LQidx;
	int SQidx;
	int stealcount;

	int RTFnum;
	int MaxWnum;

public:
	Scheduler();
	~Scheduler();

	void Simulate();

	bool LoadInput(string fileName = "test.txt");	// Load the Input file
	void MakeOutputFile();
	void makeCPU();		// Create the CPU array

	bool AddtoBLK(Process* p);	// Add Process p to BLK list
	bool AddtoTRM(Process* p);	// Add Process p to TRM list
	bool Add2aRDY(Process* p);
	bool Add2anFCFS(Process* p);
	bool NEWtoRDY(int ts);		// Distribute the new Processes arriving at ts among the Processors in the CPU
	bool BLKtoRDY();		// Moves a Process from the BLK list to the RDY list of some Processor
	bool Kill(int pid, int ts);

	void PrintCPU();		// Print the type and RDY list of the Processors
	void PrintCPU_RUN();	// Print the Processes running in the Processors
	void PrintBLK();		// Print the BLK list
	void PrintTRM();		// Print the TRM list

	void set_LQF_SQF();
	float getSTLlimit();
	void Steal(int ts);

	bool ProcessMigrationfromRRtoSJF(Process* p, Processor* processor, int timestep);
	bool ProcessMigrationfromFCFStoRR(Process* p, Processor* processor, int timestep);
	int getshoertestRDYRR();
	int getshortestRDYSJF();

	// Phase1 ONLY
};


#endif // !Scheduler_h
