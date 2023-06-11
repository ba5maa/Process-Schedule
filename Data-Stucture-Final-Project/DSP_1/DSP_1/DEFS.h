#pragma once
#ifndef DEFS_h
#define DEFS_h

#include <fstream>
#include <iostream>
using namespace std;

enum State
{
	NEWP,
	RDYP,
	RUNP,
	BLKP,
	TRMP
};

enum ProcType
{
	FCFSP = 1,
	RRP = 2,
	SJFP = 3,
	EDFP = 4
};

enum ProcState
{
	IDLE,
	BUSY,
	STOP
};

// Output Mode
enum OutMode
{
	Interactive,	// Interactive Mode
	StepbyStep,		// StepbyStep Mode
	Silent			// Silent Mode
};

// Process IO info
struct InOut
{
	int R;		// IO Request time
	int D;		// IO Duration

	// Read IO info from input file
	bool readInfo(ifstream& inFile)
	{
		char str[5];

		inFile.getline(str, 5, '(');

		inFile.getline(str, 5, ',');
		R = atoi(str);

		inFile.getline(str, 5, ')');
		D = atoi(str);

		return true;
	}
};

// Process Signal Kill info
struct KillSignal
{
	int KTime;	// Kill Time
	int PID;	// ID of Process to Kill

	// Read Kill Signal info from input file
	bool readInfo(ifstream& inFile)
	{
		inFile >> KTime >> PID;
		return true;
	}
};


#endif // !DEFS_h
