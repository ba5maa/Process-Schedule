#pragma once
#ifndef Process_h
#define Process_h

#include <iomanip>
#include <iostream>
#include <fstream>
#include "DEFS.h"
#include "Queue.h"
#include "Processor.h"
using namespace std;

class Processor;

class Process
{
private:
	int AT, PID, CT, DL;		// AT(), PID(), CT()
	int RT, TT, TRT, WT;	// RT(), TT(), TRT(), WT()
	int timeServed;			// 
	int remnBLKtime;
	int Tot_IO_D;				// Total IO Duration
	int IO_D;	// current IO Duration
	int IO_R;
	Queue<InOut> IO;		// 

	bool Killed;

	int Parent;
	int Child_1;
	int Child_2;

	int countoftime = 0;    //time process stay in blk
	State StateOfProcess;

	static int MaxPID;		// Maximum Process ID made


	// Overloading OStream operator
	friend ostream& operator<<(ostream&, const Process&);

public:
	Process();
	Process(int par, int at, int ct, int pid = MaxPID + 1);

	bool Execute(Processor* ProcPtr, int TS);
	bool handleIO();

	bool Block(Processor* ProcPtr);			// The process ask the processor to be Blocked, Return true if Blocked
	bool Terminate(Processor* ProcPtr, int ts);		// The process ask the processor to be Terminated, Return true if Terminated

	bool AddChild(Process* ch);
	bool CanHaveaChild();
	bool isForked();
	bool isKilled(bool b = false);

	int getAT() const;		// Return process arrival time
	int getDL() const;
	int getPID() const;		// Return process ID
	int getWT() const;
	int getRT() const;
	int getTT() const;
	int getTRT() const;
	bool setRT(int ts);		// Set process response time
	int rmngCT();
	int currentCT();

	bool readInfo(ifstream& inFile);	// Read process info from input file
	void PrintToFile(ofstream& outFile);

	void increment();
	int getcountoftime();

	void setStateOfProcess(State s);
	State getStateofProcess();
	InOut getIO();
	int gettimeserved();

	int getIO_R();
	int getIO_D();
	InOut dequeueBLK();

};

#endif // !Process_h
