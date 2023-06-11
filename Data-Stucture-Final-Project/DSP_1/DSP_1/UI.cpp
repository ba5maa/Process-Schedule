#include "UI.h"
#include "Scheduler.h"


UI::UI(Scheduler* skdptr)
{
	SkdPtr = skdptr;
	Mode = Interactive;
}

void UI::UpdateInterface(int ts)
{
	if (Mode == Silent)
		return;
	system("cls");

	cout << "Current Timestep: " << ts << '\n';

	cout << "------------------ RDY processes ------------------" << '\n';
	SkdPtr->PrintCPU();

	cout << "------------------ BLK processes ------------------" << '\n';
	SkdPtr->PrintBLK();

	cout << "------------------ RUN processes ------------------" << '\n';
	SkdPtr->PrintCPU_RUN();

	cout << "------------------ TRM processes ------------------" << '\n';
	SkdPtr->PrintTRM();

	if (Mode == Interactive)
	{
		cout << "PRESS ANY KEY TO MOVE TO NEXT STEP ! \n";
		system("pause > nul");
	}
	else if (Mode == StepbyStep)
		sleep(1);
}

void UI::inOutputMode()
{
	int temp{};
	cout << "\nOutput Modes: \n";
	cout << "\t1) Interactive \n";
	cout << "\t2) StepbyStep \n";
	cout << "\t3) Silent \n";
	cout << "\nSelect Output Mode(1-3): ";
	cin >> temp;

	while (true)
	{
		switch (temp) 
		{
		case 1:
			Mode = Interactive;
			return;

		case 2:
			Mode = StepbyStep;
			return;

		case 3:
			Mode = Silent;
			system("cls");
			cout << "Silent Mode....	Simulation Started... \n";
			return;

		default:
			cout << "Enter a valid number(1-3): ";
			cin >> temp;
		}
	}
}

string UI::inFileName()
{
	string str;
	cout << "Enter Input File Name: ";
	cin >> str;
	return str;
}

void UI::PrintMsg(string msg)
{
	cout << msg << '\n';
}

void UI::SimulationEnded()
{
	cout << "Simulation ends, Output file created" << '\n';
}
