#pragma once
#ifndef SJF_h
#define SJF_h

#include <iostream>
#include "Processor.h"
#include "PriQue.h"
using namespace std;

class SJF :
    public Processor
{
private:
    PriQue<Process*> RDY;

public:
    SJF(Scheduler* skdptr = NULL);
    ~SJF();

    bool ScheduleAlgo(int ts);
    bool AddtoRDY(Process* p);
    
    void PutinStop();
    void Print();

    bool RDYtoRUN();
    bool RUNtoRDY();

    int getexFT();

    Process* STEAL();

    ProcType getType();

};


#endif // !SJF_h