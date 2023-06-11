#pragma once
#ifndef FCFS_h
#define FCFS_h

#include <iostream>
#include "Processor.h"
#include "DEFS.h"
#include "LinkedList.h"
using namespace std;


class FCFS :
    public Processor
{
private:
    LinkedList<Process*> RDY;

    static int ForkP;
    static Queue<KillSignal> KillSig;
    
public:
    FCFS(Scheduler* skdptr = NULL);
    ~FCFS();

    static bool setForkP(int pro);
    static bool AddKillSig(KillSignal KS);

    bool ScheduleAlgo(int ts);
    bool AddtoRDY(Process* p);

    void PutinStop();
    void Print();

    Process* STEAL();

    //
    bool RDYtoRUN();
    bool RUNtoRDY();
    bool TerminatebyPID(int pid, int ts, bool kill = false);	// Terminates a process in the RDY list by its PID (only for FCFS processores)

    int getexFT();

    ProcType getType();

};


#endif // !FCFS_h