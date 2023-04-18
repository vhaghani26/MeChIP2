#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "limits.h"
#include "time.h"

#include "StringLib.h"
#include "MatrixLib.h"
#include "RandomLib.h"
#include "MathLib.h"
#include "MotifLib.h"
#include "SequenceLib.h"
#include "GenomeLib.h"
#include "MicroarrayLib.h"
#include "AffyLib.h"
#include "HTSequencingLib.h"

int menu_cnv_repeat2window(int argv, char **argc);

int main(int argv, char **argc)
{
	int nLen;
	int nseed;

	/* init rand */
	srand( (unsigned)time( NULL ) );
	if(strcmp(OS_SYSTEM, "WINDOWS") == 0)
	{
		nseed = (int)(rand()*1000/RAND_MAX);
	}
	else
	{
		nseed = rand()%1000;
	}
	rand_u_init(nseed);


	/* ---- */
	/* menu */
	/* ---- */
	menu_cnv_repeat2window(argv, argc);

	/* exit */
	exit(EXIT_SUCCESS);
}

int menu_cnv_repeat2window(int argv, char **argc)
{
	char strInputPath[LINE_LENGTH];
	char strOutputPath[LINE_LENGTH];
	int nW = 100;
	int nL = 10;
	int nN = 1;
	char strChrList[MED_LINE_LENGTH];
	char strChrLen[MED_LINE_LENGTH];
	int nResult;

	int iOK;
	int oOK;
	int gOK;
	int lOK;
	int wOK;
	int sOK;
	int nOK;
	int ni;

	/* ------------------------------- */
	/*    cnv_repeat2window            */
	/* -i genome sequence (*.sq) folder*/
	/* -o output                       */
	/* -g chromosome list              */
	/* -l chromosome length            */
	/* -w window size (default = 100)  */
	/* -s step multiplier (default=10) */
	/* -n step number (default = 3)    */
	/* ------------------------------- */
	if(argv < 1)
	{
		printf("Error: parameter wrong!\n");
		exit(EXIT_FAILURE);
	}
	else if(argv == 1)
	{
		printf("/* ----------------------------- */\n");
		printf("    cnv_repeat2window               \n");
		printf(" -i genome sequence (*.sq) folder\n");
		printf(" -o output                       \n");
		printf(" -g chromosome list              \n");
		printf(" -l chromosome length            \n");
		printf(" -w window size (default = 100)  \n");
		printf(" -s step multiplier (default=10) \n");
		printf(" -n step number (default = 1)    \n");
		printf(" example: \n");
		printf("    cnv_repeat2window -i /data/hg18 -o hg18_repeat_w -g /data/hg18/chrlist.txt -l /data/hg18/chrlen.txt -w 100 -s 10 -n 3\n");
		printf("/* ----------------------------- */\n");
		exit(EXIT_SUCCESS);
	}

	iOK = 0;
	oOK = 0;
	gOK = 0;
	lOK = 0;
	wOK = 0;
	sOK = 0;
	nOK = 0;

	ni = 1;
	while(ni < argv)
	{
		if(strcmp(argc[ni], "-i") == 0)
		{
			ni++;
			strcpy(strInputPath, argc[ni]);
			iOK = 1;
		}
		else if(strcmp(argc[ni], "-o") == 0)
		{
			ni++;
			strcpy(strOutputPath, argc[ni]);
			oOK = 1;
		}
		else if(strcmp(argc[ni], "-g") == 0)
		{
			ni++;
			strcpy(strChrList, argc[ni]);
			gOK = 1;
		}
		else if(strcmp(argc[ni], "-l") == 0)
		{
			ni++;
			strcpy(strChrLen, argc[ni]);
			lOK = 1;
		}
		else if(strcmp(argc[ni], "-w") == 0)
		{
			ni++;
			nW = atoi(argc[ni]);
			wOK = 1;
		}
		else if(strcmp(argc[ni], "-s") == 0)
		{
			ni++;
			nL = atoi(argc[ni]);
			sOK = 1;
		}
		else if(strcmp(argc[ni], "-n") == 0)
		{
			ni++;
			nN = atoi(argc[ni]);
			nOK = 1;
		}
		else 
		{
			printf("Error: unknown parameters!\n");
			exit(EXIT_FAILURE);
		}

		ni++;
	}

	if((iOK == 0) || (oOK == 0) || (gOK == 0) || (lOK == 0))
	{
		printf("Error: Input Parameter not correct!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		nResult = CNV_Repeat2Window_Main(strInputPath, strOutputPath, strChrList, strChrLen,
						nW, nL, nN);
	}

	return nResult;
}
