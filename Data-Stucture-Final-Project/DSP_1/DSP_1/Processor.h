#pragma once
#ifndef Processor_h
#define Processor_h

#include <iostream>
#include "Process.h"
#include "DEFS.h"
using namespace std;

class Scheduler;

class Processor
{
protected:
	Process* RUN;		// Pointer to the process running
	ProcState State;	// The state of the Processor IDLE or BUSY
	int exFT;			// Expected Finishing Time

	int timeStoped;
	static int StopTime;
	static int StopProb;

	Scheduler* SkdPtr;	// Pointer to the Scheduler

	int BUSYtime;	// Processor BUSY Time
	int IDLEtime;	// Processor IDLE Time
	int STOPtime;

	ProcType Type;

public:
	Processor(Scheduler* skdptr = NULL);
	~Processor();

	static bool setStopTime(int st);
	static bool setStopProb(int sp);

	virtual bool ScheduleAlgo(int ts) = 0; // 

	virtual bool AddtoRDY(Process* p) = 0; // it will add process to the RDY list 

	virtual bool Block(Process* p);			// Ask the Scheduler to add process *p to the BLK list
	virtual bool Terminate(Process* p);		// Ask the Scheduler to add process *p to the TRM list
	virtual bool TerminatebyPID(int pid, int ts, bool kill = false);	// Terminates a process in the RDY list by its PID (only for FCFS processores)
	virtual bool Kill(int pid, int ts);

	virtual bool isIDLE();	// Return true if the processor is IDLE(RUN = NULL)
	virtual bool isBUSY();
	virtual bool isSTOP();
	virtual void PutinStop() = 0;
	virtual int getexFT();

	virtual void Print() = 0;	// Print the processor type and RDY list
	void PrintRUN();			// Print the process running

	virtual Process* STEAL() = 0;

	int getIDLEtime();
	int getBUSYtime();
	int getSTOPtime();

	virtual ProcType getType() = 0;
	void setRUN(Process* p);
	Process* getRUN();
	void setStateofprocessor(ProcState S);
	ProcState getstateofprocessor();

	//Phase1 ONLY
	virtual bool RDYtoRUN() = 0;		// Moves a process from RDY list to RUN
	virtual bool RUNtoRDY() = 0;		// Moves the process in RUN to RDY list
	virtual bool BlockRUN();			// Ask the Scheduler to add process in RUN to the BLK list
	virtual bool TerminateRUN();		// Terminates the process in the RUN

};


#endif // !Processor_h