#pragma once
#ifndef EDF_h
#define EDF_h

#include "PriQue.h"
#include "Processor.h"
#include <iostream>
using namespace std;

class EDF :
    public Processor
{
private:
    PriQue<Process*> RDY;

public:
    EDF(Scheduler* skdptr = NULL);
    ~EDF();

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


#endif // !EDF_h

