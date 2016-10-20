#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "release.h"

using namespace std;

#define NUMTESTCASES 10
#define LATITUDE 0
#define LONGITUDE 1

int main(int argc, char *argv[])
{
	//Target Pos
	double dTargetPos [2] = {43.598204,1.413809};
	//Dron Pos
	double dDronPos[NUMTESTCASES][2] = {	{43.598151,1.413799},
											{43.598151,1.413799},
											{43.598151,1.413799},
											{43.598151,1.413799},
											{43.598120,1.413795},
											{43.598000,1.413700},
											{43.598000,1.413700},
											{43.598000,1.413700},
											{43.598056,1.413795},
											{43.598056,1.413795}};
	//Dron Speed
	float dDronSpeed[NUMTESTCASES]= {2.4, 4.1, 5.5, 6.9, 7.7, 8.6, 9.3, 11.89, 13.24, 10.58};
	//Dron Height
	float dDronHeight[NUMTESTCASES]= {9.2, 12.2, 15.5, 17.8, 20.6, 25.8, 30.4, 32.98, 38.2, 45.6};

	//Weapon System Armed
	int iSystArmed[NUMTESTCASES]= {1944, 1000, 1812, 945, 1900, 1900, 1900, 1900, 1900, 1900};

	//Weapon Armed
	int iWeaponArmed[NUMTESTCASES]= {1948, 814, 874, 1978, 1900, 1900, 1900, 1900, 1900, 1900};

	//Release Consent
	bool bReleaseConsent[NUMTESTCASES]= {false, false, false, false, true, false, false, true, false, false};

    //TestCases Execution
	for (int iIndex = 0; iIndex < NUMTESTCASES ; iIndex++){
		bool bReleaseSignal;
		bool bTestCasePassed = false;

		// Algorithm execution
		bReleaseSignal = release_calc (dTargetPos[LATITUDE],dTargetPos[LONGITUDE],dDronPos[iIndex][LATITUDE],dDronPos[iIndex][LONGITUDE],dDronSpeed[iIndex],dDronHeight[iIndex], iSystArmed[iIndex], iWeaponArmed[iIndex], 0);

		// Result checking
		if (bReleaseSignal == bReleaseConsent[iIndex])
			printf ("TestCase#%d: *** PASSED ***\n",iIndex+1);
		else
			printf ("TestCase#%d: *** NOT PASSED ***\n",iIndex+1);

		system("PAUSE");
	}

	printf ("********** EXECUTION FINISHED *************");
    system("PAUSE");
    return EXIT_SUCCESS;
}
