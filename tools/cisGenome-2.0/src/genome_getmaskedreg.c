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
#include "WorkLib.h"

int menu_getmaskedregionfromgenome(int argv, char **argc);

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
	menu_getmaskedregionfromgenome(argv, argc);

	/* exit */
	exit(EXIT_SUCCESS);
}

int menu_getmaskedregionfromgenome(int argv, char **argc)
{
	/* define */
	char strGenomePath[LINE_LENGTH];
	char strConservePath[LINE_LENGTH];
	char strCdsPath[LINE_LENGTH];
	char strSpecies[LINE_LENGTH];
	char strTargetFile[LINE_LENGTH];
	char strOutputPath[LINE_LENGTH];
		
	int ni;
	double dR,dC,dCR,dCDS;
	int dOK,rOK,cOK,crOK,cdOK,cdsOK,cdsdOK,sOK,iOK,oOK;
	int nResult;

	/* ------------------------------- */
	/* genome_getmaskedreg             */
	/* -d database                     */
	/* -r repeat cutoff                */
	/* -c conservation cutoff          */
	/* -cd conservation                */
	/* -cds cutoff                     */
	/* -cdsd coding region             */
	/* -s species                      */
	/* -i target region                */
	/* -o output path                  */
	/* ------------------------------- */
	if(argv < 1)
	{
		printf("Error: parameter wrong!\n");
		exit(EXIT_FAILURE);
	}
	else if(argv == 1)
	{
		printf("/* ----------------------------- */\n");
		printf("     genome_getmaskedreg         \n");
		printf(" -d path of genome database      \n");
		printf(" -r repeat cutoff                \n");
		printf(" -c conservation cutoff          \n");
		printf(" -cr minimum percentage of conserved bases \n");
		printf(" -cd path of conservation database \n");
		printf(" -cds CDS cutoff                 \n");
		printf(" -cdsd path of coding region database\n");
		printf(" -s species name                 \n");
		printf("		currently supporting: human, mouse, dog, cow, chicken, zebrafish\n");
		printf(" -i tab-delimited file for specifying target region \n");
		printf("		col1: seqid; \n");
		printf("		col2: chromosome in numbers, e.g. use 23 for human X, 1, 2, etc. \n");
		printf("		col3: zero-based start position in the assembly \n");
		printf("		col4: zero-based end position in the assembly \n");
		printf("		col5: strand in the assembly, '+' or '-' \n");
		printf(" -o output path \n");
		printf(" example: \n");
		printf("    genome_getmaskedreg -d /data/genomes/human/b35_hg17/ -r 0.9 -c 40 -cr 0.9 -cd /data/genomes/human/b35_hg17/conservation/phastcons/ -cds 0.9 -cdsd /data/genomes/human/b35_hg17/conservation/phastcons/cds/ -s human -i /data/genomes/human/b35_hg17/testid.txt -o testid_masked.txt\n");
		printf("/* ----------------------------- */\n");
		exit(EXIT_SUCCESS);
	}


	dR = 0.0;
	dC = 0.0;
	dCR = 0.0;
	dCDS = 0.0;
	dOK = 0;
	rOK = 0;
	cOK = 0;
	crOK = 0;
	cdOK = 0;
	cdsOK = 0;
	cdsdOK = 0;
	sOK = 0;
	iOK = 0;
	oOK = 0;
	
	/* set default */
	ni = 1;
	while(ni < argv)
	{
		if(strcmp(argc[ni], "-d") == 0)
		{
			ni++;
			strcpy(strGenomePath, argc[ni]);
			dOK = 1;
		}
		else if(strcmp(argc[ni], "-r") == 0)
		{
			ni++;
			dR = atof(argc[ni]);
			rOK = 1;
		}
		else if(strcmp(argc[ni], "-c") == 0)
		{
			ni++;
			dC = atof(argc[ni]);
			cOK = 1;
		}
		else if(strcmp(argc[ni], "-cr") == 0)
		{
			ni++;
			dCR = atof(argc[ni]);
			crOK = 1;
		}
		else if(strcmp(argc[ni], "-cd") == 0)
		{
			ni++;
			strcpy(strConservePath, argc[ni]);
			cdOK = 1;
		}
		else if(strcmp(argc[ni], "-cds") == 0)
		{
			ni++;
			dCDS = atof(argc[ni]);
			cdsOK = 1;
		}
		else if(strcmp(argc[ni], "-cdsd") == 0)
		{
			ni++;
			strcpy(strCdsPath, argc[ni]);
			cdsdOK = 1;
		}
		else if(strcmp(argc[ni], "-s") == 0)
		{
			ni++;
			strcpy(strSpecies, argc[ni]);
			sOK = 1;
		}
		else if(strcmp(argc[ni], "-i") == 0)
		{
			ni++;
			strcpy(strTargetFile, argc[ni]);
			iOK = 1;
		}
		else if(strcmp(argc[ni], "-o") == 0)
		{
			ni++;
			strcpy(strOutputPath, argc[ni]);
			oOK = 1;
		}
		else 
		{
			printf("Error: unknown parameters!\n");
			exit(EXIT_FAILURE);
		}

		ni++;
	}

	if( (dOK == 0) ||  (sOK == 0) || (iOK == 0) || (oOK == 0) || ( (rOK == 0) && (cOK == 0) && (cdsOK == 0) ) )
	{
		printf("Error: Input Parameter not correct!\n");
		exit(EXIT_FAILURE);
	}

    if( (cOK == 1) || (crOK == 1) || (cdOK == 1))
	{
		if( (cOK == 0) || (crOK == 0) || (cdOK == 0))
		{
			printf("Error: Input Parameter not correct, no conservation path was specified!\n");
			exit(EXIT_FAILURE);
		}
	}

	if( (cdsOK == 1) || (cdsdOK == 1))
	{
		if( (cdsOK == 0) || (cdsdOK == 0) )
		{
			printf("Error: Input Parameter not correct, no cds path was specified!\n");
			exit(EXIT_FAILURE);
		}
	}

	nResult = Genome_Code_4bit_GetMaskedReg_Main(strGenomePath, strSpecies,
			rOK, dR, cOK, dC, dCR, strConservePath, cdsOK, dCDS, strCdsPath,
			strTargetFile, strOutputPath); 

	return nResult;
}