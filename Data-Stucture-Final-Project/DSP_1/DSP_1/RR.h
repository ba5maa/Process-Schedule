#pragma once
#ifndef RR_h
#define RR_h

#include <iostream>
#include "Processor.h"
#include "Queue.h"
using namespace std;

class RR :
    public Processor
{
private:
    Queue<Process*> RDY;
    static int TSlice;

    int TimeinRUN;

public:
    RR(Scheduler* skdptr = NULL);
    ~RR();

    static bool setTSlice(int ts);

    bool ScheduleAlgo(int ts);
    bool AddtoRDY(Process* p);
    
    void PutinStop();
    void Print();

    bool RDYtoRUN();
    bool RUNtoRDY();

    int getexFT();

    Process* STEAL();

    ProcType getType();
    void setTimeinRun(int x);

};


#endif // !RR_h