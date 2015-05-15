/*******************************************************************************
/*  Title: Assignment 8 - create.c
/*  Author:  Stephen Vanderende
/*  Class COMP 2717
/*  Date: 03/12/12
/******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#define MAXCHAR 30
#define BUFFER 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char sAccountNumber[ MAXCHAR ];
	char sAccountName[ MAXCHAR ];
	double dAccountBalance;
	double dLastPaymentAmount;
} Account;

void checkCommandArguments( int iNumber );
void checkInputFile( char *sFileName );
void checkOutputFile( char *sFileName );
void writeOutputFile( char *sFileData );

FILE *fpData;
FILE *fpOutput;


int main( int argc, char *argv[] )
{
	int iStringLength = 0;
	char sFileString[ BUFFER ];
	
	checkCommandArguments( argc );
	checkInputFile( argv[1] );
	checkOutputFile( argv[2] );

	fgets( sFileString, BUFFER, fpData );
	rewind( fpOutput );

	printf("%10s%20s%20s%20s\n", "Account Number", "Account Name", "Account Balance", "Last Payment Amount");
	printf("--------------------------------------------------------------------------\n");
	while ( !feof( fpData ) )
	{
		iStringLength = strlen(sFileString);
		iStringLength--;
	
		if( sFileString[iStringLength] == '\n' )
		{
			sFileString[iStringLength] = '\0';
		}

		writeOutputFile( sFileString );

		fgets( sFileString, BUFFER, fpData ); 
	}

	fclose(fpData);
	fclose(fpOutput);

	return 0;
}

// Check number of command line arguments
void checkCommandArguments( int iNumber )
{
	if ( iNumber < 3 )
	{
		printf("There are too few arguments.\n");
		exit( EXIT_FAILURE );
	}

	if ( iNumber > 3 )
	{
		printf("There are too many arguments.\n");
		exit( EXIT_FAILURE );
	}
}

// Check if the input file exists
void checkInputFile( char *sFileName )
{
	if( ( fpData = fopen(sFileName, "r" ) ) == NULL )
	{
		printf("%s could not be opened.\n", sFileName);
		exit( EXIT_FAILURE );
	}
}

// Check if the output file exists
void checkOutputFile( char *sFileName )
{
	char cAnswer;

	if( ( fpOutput = fopen(sFileName, "rb") ) != NULL )
	{
		printf("%s file exists.  Overwrite? y or n ", sFileName );
		scanf( "%c", &cAnswer );
		fflush;
		fclose( fpOutput );

		if( cAnswer == 'y' )
		{
			if( ( fpOutput = fopen(sFileName, "wb") ) == NULL )
			{
				printf("%s could not be opened.\n", sFileName );
				exit( EXIT_FAILURE );
			}
		}
		else
		{
			printf( "Cannot overwrite %s file, exiting program.\n", sFileName );
			exit( EXIT_FAILURE );
		}
	}
	else
	{
		if( ( fpOutput = fopen(sFileName, "wb") ) == NULL )
		{
			printf("%s could not be opened.\n", sFileName );
			exit( EXIT_FAILURE );
		}
	}
}

// Write the csv file data to the binary file
void writeOutputFile( char *sFileData )
{
	Account temp = {"","",0.0,0.0};
	char *pToken = NULL;

	pToken = strtok(sFileData, "," );

	strcpy( temp.sAccountNumber, pToken );
	printf( "%4s", temp.sAccountNumber );
	pToken = strtok(NULL, "," );

	strcpy( temp.sAccountName, pToken );
	printf("%32s", temp.sAccountName);
	pToken = strtok(NULL, "," );

	temp.dAccountBalance = atof( pToken );
	printf("%18.2f", temp.dAccountBalance);
	pToken = strtok(NULL, "," );

	temp.dLastPaymentAmount = atof( pToken );
	printf("%20.2f\n", temp.dLastPaymentAmount);

	fwrite( &temp, sizeof( Account ), 1, fpOutput );
}

