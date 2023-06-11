#pragma once

#include<iostream>
#include <string>
#include "DEFS.h"

#ifdef __unix__            
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

using namespace std;
class Scheduler;

class UI
{
	Scheduler* SkdPtr;	// Pointer to the Scheduler
	OutMode Mode;		// The Output Mode entered by the User

public:
	UI(Scheduler* skdptr);

	void UpdateInterface(int ts);	// Update the Interface according to the Output Mode

	void inOutputMode();		// Ask the User to enter the Output Mode
	string inFileName();		// Ask the User to enter the Input file name
	void PrintMsg(string msg);	// Printes a Message to the User
	void SimulationEnded();

};

